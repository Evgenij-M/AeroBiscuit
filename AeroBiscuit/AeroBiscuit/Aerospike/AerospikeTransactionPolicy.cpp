#include "AerospikeTransactionPolicy.hpp"

namespace asw
{
	TransactionPolicy::TransactionPolicy(const uint32_t time_to_live, const uint32_t total_timeout) : time_to_live{time_to_live}, total_timeout{total_timeout}
	{
	
	}
	
} // asw
