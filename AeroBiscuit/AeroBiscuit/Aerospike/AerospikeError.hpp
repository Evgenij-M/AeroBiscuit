#ifndef AEROSPIKEERROR_HPP
#define AEROSPIKEERROR_HPP

#include <iostream>

#include <aerospike/as_error.h>

namespace asw
{
	class Error final
	{
	public:
		
		enum class Status : int8_t
		{
			Ok = 0,
			ClientError,
			ServerError,
			UdfError
		};
		
		Error() = default;
		Error(int16_t code, std::string message, std::string function, std::string file, uint32_t line, bool in_doubt);
		explicit Error(const as_error* error);
		
		const bool operator==(const Status &other_status) const;
		const bool operator!=(const Status &other_status) const;
		const bool operator>(const Status &other_status) const;
		const bool operator<(const Status &other_status) const;
		const bool operator>=(const Status &other_status) const;
		const bool operator<=(const Status &other_status) const;
	
	public:
		
		int16_t code = 0;
		Status status = Status::Ok;
		std::string message;
		std::string function;
		std::string file;
		uint32_t line = 0;
		bool in_doubt = false;
	};
	
} // asw

#endif //AEROSPIKEERROR_HPP
