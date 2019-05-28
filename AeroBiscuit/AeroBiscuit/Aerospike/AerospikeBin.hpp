#ifndef AEROSPIKEBIN_HPP
#define AEROSPIKEBIN_HPP

#include <iostream>

#include <aerospike/as_bin.h>

namespace asw
{
	class BaseBin
	{
	public:
		
		enum class Type : int8_t
		{
			Base = 0,
			Double,
			Int64,
			String
		};
		
		explicit BaseBin(std::string name);
		explicit BaseBin(const as_bin* bin);
		virtual ~BaseBin() = default;
		
		const std::string& Name() const;
		const enum Type& Type() const;
		
	protected:
		
		BaseBin() = default;
		BaseBin(std::string name, enum Type type);
		
		enum Type m_type = Type::Base;
		std::string m_name;
	};
	
	class DoubleBin final : public BaseBin
	{
	public:
		
		DoubleBin() = delete;
		DoubleBin(const std::string& name, double value);
		explicit DoubleBin(const as_bin* bin);
		
		const double Value() const;
	
	private:
		
		double m_value = 0;
	};
	
	class Int64Bin final : public BaseBin
	{
	public:
		
		Int64Bin() = delete;
		Int64Bin(const std::string& name, int64_t value);
		explicit Int64Bin(const as_bin* bin);
		
		const int64_t Value() const;
	
	private:
		
		int64_t m_value = 0;
	};
	
	class StringBin final : public BaseBin
	{
	public:

		StringBin() = delete;
		StringBin(const std::string& name, std::string value);
		explicit StringBin(const as_bin* bin);
		
		const std::string& Value() const;
	
	private:
		
		std::string m_value;
	};

} // asw

#endif //AEROSPIKEBIN_HPP
