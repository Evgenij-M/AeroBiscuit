#include "AerospikeClient.hpp"

#include <functional> // std::function

#include <aerospike/aerospike_key.h>
#include <aerospike/as_record_iterator.h>
#include <aerospike/as_error.h>
#include <aerospike/as_record.h>
#include <aerospike/as_status.h>

namespace asw
{
	Client::~Client()
	{
		Disconnect();
	}

	bool Client::Connect(const Configuration& config)
	{
		if (ConnectionStatus == ClientConnectionStatus::Connected)
		{
			return false;
		}
		
		m_as_client = std::make_unique<aerospike>();
		
		as_error as_error;
		
		as_policy_event as_event_policy;
		as_policy_event_init(&as_event_policy);

		if (as_create_event_loops(&as_error, &as_event_policy, m_cpu_core_num, nullptr) != AEROSPIKE_OK)
		{
			std::cerr << "Error: Failed to create event loop" << std::endl;
			
			return false;
		}
		
		as_config as_config;
		as_config_init(&as_config);
		
		for (const auto& host : config.hosts)
		{
			as_config_add_host(&as_config, host.address.c_str(), host.port);
		}
		
		as_config.async_max_conns_per_node = m_cpu_core_num * m_transaction_queue_limit;
		
		if (aerospike_init(m_as_client.get(), &as_config) == nullptr)
		{
			return false;
		}
		
		if (aerospike_connect(m_as_client.get(), &as_error) != AEROSPIKE_OK)
		{
			std::cerr << "Error: Failed to connect - " << as_error.code << " " << as_error.message << std::endl;
			
			return false;
		}
		
		ConnectionStatus = ClientConnectionStatus::Connected;
		
		std::function<void()> process_transaction_queue = [&]()
		{
			ProcessTransactionQueue();
		};
		
		if (m_transaction_queue_thread.joinable())
		{
			m_transaction_queue_thread.join();
		}
		
		m_transaction_queue_thread = std::thread(process_transaction_queue);
		
		return true;
	}
	
	bool Client::Disconnect()
	{
		if (ConnectionStatus == ClientConnectionStatus::Disconnected)
		{
			return false;
		}
		
		as_error as_error;
		
		if (aerospike_close(m_as_client.get(), &as_error) != AEROSPIKE_OK)
		{
			std::cerr << "Error: Failed to close aerospike - " << as_error.code << " " << as_error.message << std::endl;
			
			return false;
		}
		
		aerospike_destroy(m_as_client.get());
		
		as_event_close_loops();
		
		m_transaction_queue.LockQueueMutex();
		
		ConnectionStatus = ClientConnectionStatus::Disconnected;
		
		m_transaction_queue.UnlockQueueMutex();
		
		m_transaction_queue.NotifyQueue();
		
		if (m_transaction_queue_thread.joinable())
		{
			m_transaction_queue_thread.join();
		}
		
		return true;
	}
	
	bool Client::IsConnected()
	{
		return ConnectionStatus == ClientConnectionStatus::Connected;
	}

	void Client::ProcessTransactionQueue()
	{
		const std::function<bool()> urgent_exit = [&]()
		{
			return ConnectionStatus != ClientConnectionStatus::Connected;
		};
		
		while (ConnectionStatus == ClientConnectionStatus::Connected)
		{
			ClientStatus = ClientStatus::WaitingForTransaction;
			
			std::shared_ptr<Transaction> transaction;
			m_transaction_queue.WaitAndPop(transaction, urgent_exit);
			
			if (ConnectionStatus != ClientConnectionStatus::Connected)
			{
				MakeAsyncTransaction(transaction);
				
				break;
			}
			
			ClientStatus = ClientStatus::Busy;

			++m_transaction_queue_size;

			ProcessTransaction(transaction, nullptr);
			
			if (m_transaction_queue_size >= m_transaction_queue_limit)
			{
				ClientStatus = ClientStatus::WaitingForQueue;
				
				std::unique_lock lock(m_queue_limit_mutex);
				m_queue_limit_condition_variable.wait(lock, [&]()
				{
					return m_transaction_queue_size < m_transaction_queue_limit;
				});
				
			}

		}
		
		ClientStatus = ClientStatus::Idle;
	}
	
	void Client::ProcessTransaction(const std::shared_ptr<Transaction>& transaction, as_event_loop* event_loop)
	{
		as_error as_error;
		
		as_operations as_operations;
		as_operations_inita(&as_operations, transaction->GetOperations().size());
		
		for (const auto& operation : transaction->GetOperations())
		{
			operation.Apply(&as_operations);
		}
		
		as_operations.ttl = transaction->policy.time_to_live;
		
		as_key as_key;
		as_key_init(&as_key, transaction->key.ns.c_str(), transaction->key.set.c_str(), transaction->key.value.c_str());

		as_policy_operate as_policy_operate;
		as_policy_operate_init(&as_policy_operate);
		
		as_policy_operate.base.total_timeout = transaction->policy.total_timeout;

		if (aerospike_key_operate_async(m_as_client.get(), &as_error, &as_policy_operate, &as_key, &as_operations, &Client::OpsCallback, transaction.get(), event_loop, nullptr) != AEROSPIKE_OK)
		{
			OpsCallback(&as_error, nullptr, nullptr, event_loop);
		}
		
		as_operations_destroy(&as_operations);
		as_key_destroy(&as_key);
	}
	
	void Client::OpsCallback(as_error* as_error, as_record* as_record, void* user_data, as_event_loop* as_event_loop)
	{
		auto transaction = static_cast<Transaction*>(user_data);
		
		const Error error{as_error};
		const Record record{as_record};

		transaction->callback.Execute(error, record);
		transaction->Delete();

		Client::GetInstance().m_transaction_queue_size--;
		Client::GetInstance().m_queue_limit_condition_variable.notify_one();
	}
	
	bool Client::MakeAsyncTransaction(const std::shared_ptr<Transaction>& transaction)
	{
		if (transaction != nullptr && not transaction->GetOperations().empty())
		{
			m_transaction_queue.Push(transaction);
			
			return true;
		}
		
		return false;
	}
	
} // asw
