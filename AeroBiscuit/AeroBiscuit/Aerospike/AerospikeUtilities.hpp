#ifndef AEROSPIKEUTILITIES_HPP
#define AEROSPIKEUTILITIES_HPP

#include <iostream>

#include <aerospike/as_record.h>
#include <aerospike/as_bin.h>

namespace asw
{
	class Utilities final
	{
	public:
		
		Utilities() = delete;
		
		static void DumpRecord(const as_record* record);
		static void DumpBin(const as_bin* bin);
		
	};
	
} // asw

#endif //AEROSPIKEUTILITIES_HPP
