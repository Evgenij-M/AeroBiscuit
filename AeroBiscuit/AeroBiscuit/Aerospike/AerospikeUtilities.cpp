#include "AerospikeUtilities.hpp"

#include <aerospike/as_record_iterator.h>

namespace asw
{
	void Utilities::DumpRecord(const as_record* record)
	{
		if (not record)
		{
			std::cerr << "Error: DumpRecord() - Null record" << std::endl;
			
			return;
		}
		
		if (record->key.valuep)
		{
			const char* key_value = as_val_tostring(record->key.valuep);

			std::cout << "Key: " << key_value << std::endl;
			
			delete key_value;
		}
		
		const uint16_t bins_size = as_record_numbins(record);
		
		std::cout << "Generation: " << record->gen << ", time_to_live " << record->ttl << ", " << bins_size << " bins" << std::endl;
		
		as_record_iterator it;
		as_record_iterator_init(&it, record);
		
		while (as_record_iterator_has_next(&it))
		{
			DumpBin(as_record_iterator_next(&it));
		}
		
		as_record_iterator_destroy(&it);
	}
	
	void Utilities::DumpBin(const as_bin* bin)
	{
		if (not bin)
		{
			std::cerr << "Error: DumpBin() - Null bin" << std::endl;
			
			return;
		}
		
		const char* bin_value = as_val_tostring(as_bin_get_value(bin));

		std::cout << "Bin: " << as_bin_get_name(bin) << " - " << bin_value << std::endl;
		
		delete bin_value;
	}
	
} // asw
