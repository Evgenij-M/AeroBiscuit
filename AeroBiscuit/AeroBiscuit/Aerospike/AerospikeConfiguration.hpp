#ifndef AEROSPIKECONFIGURATION_HPP
#define AEROSPIKECONFIGURATION_HPP

#include <iostream>
#include <vector> // std::vector

namespace asw
{
	class Host final
	{
	public:
		
		Host() = delete;
		Host(std::string address, uint16_t port);
	
	public:
		
		std::string address;
		uint16_t port;
	};
	
	class Configuration final
	{
	public:
		
		Configuration() = delete;
		explicit Configuration(std::vector<Host> hosts);
	
	public:
		
		std::vector<Host> hosts;
	};
	
} // asw

#endif //AEROSPIKECONFIGURATION_HPP
