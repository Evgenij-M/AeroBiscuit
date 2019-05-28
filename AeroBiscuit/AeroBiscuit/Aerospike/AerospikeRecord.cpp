#include "AerospikeRecord.hpp"

#include <aerospike/as_record_iterator.h>

namespace asw
{
	Record::Record(Key key, uint32_t ttl) : key{std::move(key)}, time_to_live{ttl}
	{
	
	}
	
	Record::Record(const as_record* record)
	{
		if (record)
		{
			const Key key{&record->key};
			
			*this = Record(key, record->ttl);
			
			as_record_iterator it;
			as_record_iterator_init(&it, record);
			
			while (as_record_iterator_has_next(&it))
			{
				const as_bin* bin = as_record_iterator_next(&it);
				
				switch (as_val_type(as_bin_get_value(bin)))
				{
					case AS_DOUBLE:
					{
						const std::shared_ptr<DoubleBin> bin_ptr = std::make_shared<DoubleBin>(bin);
						
						AddBin(bin_ptr);
						
						break;
					}
					
					case AS_INTEGER:
					{
						const std::shared_ptr<Int64Bin> bin_ptr = std::make_shared<Int64Bin>(bin);
						
						AddBin(bin_ptr);
			
						break;
					}
					
					case AS_STRING:
					{
						const std::shared_ptr<StringBin> bin_ptr = std::make_shared<StringBin>(bin);
						
						AddBin(bin_ptr);
				
						break;
					}
					
					default:
					{
						const std::shared_ptr<BaseBin> bin_ptr = std::make_shared<BaseBin>(bin);
						
						AddBin(bin_ptr);
	
						break;
					}
					
				}
				
			}
			
			as_record_iterator_destroy(&it);
		}
		
	}
	
	const std::vector<std::shared_ptr<BaseBin>>& Record::GetBins() const
	{
		return m_bins;
	}
	
	void Record::AddBin(std::shared_ptr<BaseBin> bin)
	{
		m_bins.emplace_back(std::move(bin));
	}
	
} // asw
