#include "AerospikeCallback.hpp"

#include <boost/bind.hpp> // boost::bind()

namespace asw
{
	BaseCallback::BaseCallback(std::function<void(Error, Record)> callback) : m_callback{std::move(callback)}
	{
	
	}
	
	void BaseCallback::Execute(Error error, Record record)
	{
		m_callback(std::move(error), std::move(record));
	}
	
	
	AsioCallback::AsioCallback(std::shared_ptr<boost::asio::io_service> io_service, const std::function<void(Error, Record)> &callback) : BaseCallback(callback), m_io_service{std::move(io_service)}
	{
	
	}
	
	void AsioCallback::Execute(Error error, Record record)
	{
		m_io_service->post(boost::bind(m_callback, std::move(error), std::move(record)));
	}
	
} // asw
