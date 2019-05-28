#include <iostream>
#include <chrono>
#include <thread>

#include <boost/asio.hpp>

#include "AerospikeClient.hpp"
#include "AerospikeTransaction.hpp"

std::atomic_uint32_t counter = 0;

void AsioThread(const std::shared_ptr<boost::asio::io_service>& io_service)
{
	io_service->run();
}

void AsioCallbackFunc(const asw::Error &error, const asw::Record &record)
{
	if (error == asw::Error::Status::Ok)
	{
		++counter;
		
		if (not record.GetBins().empty())
		{
			for (const auto& bin : record.GetBins())
			{
				switch (bin->Type())
				{
					case asw::BaseBin::Type::Double :
					{
						auto double_bin = dynamic_cast<const asw::DoubleBin*>(bin.get());

						std::cout << "[DBIN:" << double_bin->Name() << "|" << double_bin->Value() << "]"<< std::endl;

						break;
					}

					case asw::BaseBin::Type::Int64 :
					{
						auto int64_bin = dynamic_cast<const asw::Int64Bin*>(bin.get());

						std::cout << "[IBIN:" << int64_bin->Name() << "|" << int64_bin->Value() << "]"<< std::endl;

						break;
					}

					case asw::BaseBin::Type::String :
					{
						auto string_bin = dynamic_cast<const asw::StringBin*>(bin.get());

						std::cout << "[SBIN:" << string_bin->Name() << "|" << string_bin->Value() << "]"<< std::endl;

						break;
					}

					default:
					{
						std::cout << "[BBIN:" << bin->Name() << "]"<< std::endl;

						break;
					}

				}
				
			}
			
		}
		
	}
	else
	{
		std::cerr << "[ERR: " << error.code << " | " << error.message << "]" << std::endl;
	}
	
}

int main(int argc, const char * argv[])
{
	auto io_service = std::make_shared<boost::asio::io_service>();
	auto work = std::make_shared<boost::asio::io_service::work>(*io_service);

	std::thread loop{AsioThread, io_service};

	asw::Client& client = asw::Client::GetInstance();
	asw::Configuration config{{asw::Host{"HOST", 3000}}};
	
	if (not client.Connect(config))
	{
		return 1;
	}

	const uint32_t trans_count{100000};
	
	const auto start_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	for (uint32_t i = 0; i < trans_count; ++i)
	{
		const asw::AsioCallback asio_callback{io_service, AsioCallbackFunc};
		const asw::TransactionPolicy transaction_policy(20);
		
		const std::shared_ptr<asw::Transaction> transaction = asw::Transaction::NewTransaction(asw::Key{"qa", "load", "key_" + std::to_string(i)}, transaction_policy, asio_callback);

		transaction->AddOperation(asw::WriteOperation{asw::StringBin{"test_bin_name", "test_bin_value"}});
		//transaction->AddOperation(ReadOperation{"test_opr"});
	
		client.MakeAsyncTransaction(transaction);
	}
	
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	client.Disconnect();
	
	const auto end_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	work.reset();
	loop.join();

	std::cout << "\nTime: " << end_timestamp.count() - start_timestamp.count() << "ms" << std::endl;
	std::cout << "[" << counter << "]" << std::endl;

	return 0;
}
