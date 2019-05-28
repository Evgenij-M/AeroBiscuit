#ifndef AEROSPIKEKEY_HPP
#define AEROSPIKEKEY_HPP

#include <iostream>

#include <aerospike/as_key.h>

namespace asw
{
	class Key final
	{
	public:
		
		Key() = default;
		Key(std::string ns, std::string set, std::string value);
		explicit Key(const as_key* key);
	
	public:
		
		std::string ns;
		std::string set;
		std::string value;
	};
	
} // asw

#endif //AEROSPIKEKEY_HPP
