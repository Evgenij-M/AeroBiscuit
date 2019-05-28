#ifndef AEROSPIKECLIENT_HPP
#define AEROSPIKECLIENT_HPP

#include <iostream>
#include <memory> // std::unique_ptr, std::shared_ptr
#include <condition_variable> // std::condition_variable
#include <mutex> // std::mutex
#include <thread> // std::thread, std::thread::hardware_concurrency()

#include <aerospike/aerospike.h> // aerospike
#include <aerospike/as_event.h> // as_event_loop

#include "AerospikeTransaction.hpp"
#include "AerospikeQueue.hpp"
#include "AerospikeConfiguration.hpp"

namespace asw
{
	class Client final
	{
	public:
		
		static Client &GetInstance()
		{
			static Client instance;
			
			return instance;
		}

		Client(const Client&) = delete;
		void operator=(const Client&) = delete;
		
		bool Connect(const Configuration& config);
		bool Disconnect();
		
		bool IsConnected();

		bool MakeAsyncTransaction(const std::shared_ptr<Transaction>& transaction);
		
	private:
		
		Client() = default;
		~Client();
		
		void ProcessTransactionQueue();
		void ProcessTransaction(const std::shared_ptr<Transaction>& transaction, as_event_loop* event_loop);
		static void OpsCallback(as_error* as_error, as_record* as_record, void* user_data, as_event_loop* as_event_loop);
	
	private:
		
		enum class ClientConnectionStatus : uint8_t
		{
			Disconnected = 0,
			Connected
			
		} ConnectionStatus = ClientConnectionStatus::Disconnected;
		
		enum class ClientStatus : uint8_t
		{
			Idle = 0,
			Busy,
			WaitingForTransaction,
			WaitingForQueue
			
		} ClientStatus = ClientStatus::Idle;
	
	private:
		
		const uint32_t m_transaction_queue_limit = 1000;
		std::atomic_uint32_t m_transaction_queue_size = 0;
		const uint32_t m_cpu_core_num = std::thread::hardware_concurrency();
		
		mutable std::mutex m_queue_limit_mutex;
		std::condition_variable m_queue_limit_condition_variable;
		std::unique_ptr<aerospike> m_as_client;
		ConcurrentQueue<std::shared_ptr<Transaction>> m_transaction_queue;
		std::thread m_transaction_queue_thread;
	};
	
} // asw

#endif //AEROSPIKECLIENT_HPP
