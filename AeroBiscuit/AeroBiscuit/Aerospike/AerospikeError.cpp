#include "AerospikeError.hpp"

namespace asw
{
	Error::Error(int16_t code, std::string message, std::string function, std::string file, uint32_t line, bool in_doubt) : code{code}, message{std::move(message)}, function{std::move(function)}, file{std::move(file)}, line{line}, in_doubt{in_doubt}
	{
		if (code == 0)
		{
			status = Status::Ok;
		}
		else if (code > 3000)
		{
			status = Status::UdfError;
		}
		else if (code > 0)
		{
			status = Status::ServerError;
		}
		else if (code < 0)
		{
			status = Status::ClientError;
		}
		
	}
	
	Error::Error(const as_error* error)
	{
		if (error)
		{
			*this = Error(error->code, error->message, error->func, error->file, error->line, error->in_doubt);
		}
	
	}
	
	const bool Error::operator==(const Status &other_status) const
	{
		return this->status == other_status;
	}
	
	const bool Error::operator!=(const Status &other_status) const
	{
		return this->status != other_status;
	}
	
	const bool Error::operator>(const Status &other_status) const
	{
		return this->status > other_status;
	}
	
	const bool Error::operator<(const Status &other_status) const
	{
		return this->status < other_status;
	}
	
	const bool Error::operator>=(const Status &other_status) const
	{
		return this->status >= other_status;
	}
	
	const bool Error::operator<=(const Status &other_status) const
	{
		return this->status <= other_status;
	}
	
} // asw
