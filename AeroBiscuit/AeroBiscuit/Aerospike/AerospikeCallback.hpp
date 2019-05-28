#ifndef AEROSPIKECALLBACK_HPP
#define AEROSPIKECALLBACK_HPP

#include <iostream>
#include <memory> // std::shared_ptr
#include <functional> // std::function

#include <boost/asio.hpp> // boost::asio::io_service

#include "AerospikeError.hpp"
#include "AerospikeRecord.hpp"

namespace asw
{
	class BaseCallback
	{
	public:
		
		BaseCallback() = delete;
		virtual ~BaseCallback() = default;
		
		virtual void Execute(Error error, Record record);
	
	protected:
		
		explicit BaseCallback(std::function<void(Error, Record)> callback);
		
		std::function<void(Error, Record)> m_callback;
	};
	
	class AsioCallback final : public BaseCallback
	{
	public:
		
		AsioCallback() = delete;
		AsioCallback(std::shared_ptr<boost::asio::io_service> io_service, const std::function<void(Error, Record)> &callback);
		
		void Execute(Error error, Record record) override;
	
private:

std::shared_ptr<boost::asio::io_service> m_io_service;
};

} // asw

#endif //AEROSPIKECALLBACK_HPP
