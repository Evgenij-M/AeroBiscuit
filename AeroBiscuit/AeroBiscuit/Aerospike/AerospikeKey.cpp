#include "AerospikeKey.hpp"

namespace asw
{
	Key::Key(std::string ns, std::string set, std::string value) : ns{std::move(ns)}, set{std::move(set)}, value{std::move(value)}
	{
	
	}
	
	Key::Key(const as_key* key)
	{
		if (key && key->valuep)
		{
			*this = Key(key->ns, key->set, key->value.string.value);
		}
		
	}
	
} // asw
