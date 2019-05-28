#include "AerospikeConfiguration.hpp"

namespace asw
{
	Host::Host(std::string address, const uint16_t port) : address{std::move(address)}, port{port}
	{
	
	}
	
	Configuration::Configuration(std::vector<Host> hosts) : hosts{std::move(hosts)}
	{

	}
	
} // asw
