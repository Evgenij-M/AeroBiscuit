#include "AerospikeBin.hpp"

namespace asw
{
	BaseBin::BaseBin(std::string name) : m_name{std::move(name)}
	{
	
	}
	
	BaseBin::BaseBin(std::string name, const enum BaseBin::Type type) : m_name{std::move(name)}, m_type{type}
	{
	
	}
	
	BaseBin::BaseBin(const as_bin* bin)
	{
		if (bin)
		{
			*this = BaseBin(as_bin_get_name(bin));
		}
		
	}
	
	const std::string& BaseBin::Name() const
	{
		return m_name;
	}
	
	const enum BaseBin::Type& BaseBin::Type() const
	{
		return m_type;
	}
	
	
	DoubleBin::DoubleBin(const std::string& name, const double value) : BaseBin(name, Type::Double), m_value{value}
	{
	
	}
	
	DoubleBin::DoubleBin(const as_bin* bin)
	{
		if (bin && bin->valuep)
		{
			const double value = bin->value.dbl.value;
			*this = DoubleBin(as_bin_get_name(bin), value);
		}
		
	}

	const double DoubleBin::Value() const
	{
		return m_value;
	}
	
	
	Int64Bin::Int64Bin(const std::string& name, const int64_t value) : BaseBin(name, Type::Int64), m_value{value}
	{
	
	}
	
	Int64Bin::Int64Bin(const as_bin* bin)
	{
		if (bin && bin->valuep)
		{
			const int64_t value = bin->value.integer.value;
			*this = Int64Bin(as_bin_get_name(bin), value);
		}
		
	}

	const int64_t Int64Bin::Value() const
	{
		return m_value;
	}
	
	
	StringBin::StringBin(const std::string& name, std::string value) : BaseBin(name, Type::String), m_value{std::move(value)}
	{
	
	}
	
	StringBin::StringBin(const as_bin* bin)
	{
		if (bin && bin->valuep)
		{
			const char* value = bin->value.string.value;
			*this = StringBin(as_bin_get_name(bin), value);
		}
		
	}

	const std::string& StringBin::Value() const
	{
		return m_value;
	}
	
} // asw
