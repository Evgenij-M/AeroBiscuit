#ifndef AEROSPIKERECORD_HPP
#define AEROSPIKERECORD_HPP

#include <iostream>
#include <vector>
#include <memory> // std::unique_ptr, std::shared_ptr

#include <aerospike/as_record.h>

#include "AerospikeKey.hpp"
#include "AerospikeBin.hpp"

namespace asw
{
	class Record final
	{
	public:
		
		Record() = default;
		Record(Key key, uint32_t ttl);
		explicit Record(const as_record* record);
		
		const std::vector<std::shared_ptr<BaseBin>>& GetBins() const;
		void AddBin(std::shared_ptr<BaseBin> bin);
		
	public:
		
		Key key;
		uint32_t time_to_live = 0;
	
	private:
		
		std::vector<std::shared_ptr<BaseBin>> m_bins;
	};
	
} // asw

#endif //AEROSPIKERECORD_HPP
