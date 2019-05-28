#ifndef AEROSPIKETRANSACTIONPOLICY_HPP
#define AEROSPIKETRANSACTIONPOLICY_HPP

#include <iostream>

namespace asw
{
	class TransactionPolicy final
	{
	public:
		
		TransactionPolicy() = default;
		explicit TransactionPolicy(uint32_t ttl, uint32_t total_timeout = 5000);
	
	public:
		
		uint32_t time_to_live = 0;
		uint32_t total_timeout = 5000;
	};
	
} // asw

#endif //AEROSPIKETRANSACTIONPOLICY_HPP
