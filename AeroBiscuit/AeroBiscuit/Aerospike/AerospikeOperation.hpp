#ifndef AEROSPIKEOPERATION_HPP
#define AEROSPIKEOPERATION_HPP

#include <iostream>
#include <functional> // std::function

#include <aerospike/as_operations.h>

#include "AerospikeBin.hpp"

namespace asw
{
	class BaseOperation
	{
	public:
		
		virtual ~BaseOperation() = default;
		
		virtual void Apply(as_operations* operations) const;
	
	protected:
		
		BaseOperation() = default;
		
		std::function<void(as_operations* operations)> m_apply = [](as_operations*) {};
	};
	
	
	class BinOperation : public BaseOperation
	{
	public:
		
		BinOperation() = delete;
		
		const BaseBin& Bin() const;
	
	protected:
		
		explicit BinOperation(const BaseBin& bin);
		
		BaseBin m_bin;
	};
	
	class RecordOperation : public BaseOperation
	{
	protected:
		
		RecordOperation() = default;
		
	};
	
	
	class ReadOperation final : public BinOperation
	{
	public:
		
		ReadOperation() = delete;
		
		explicit ReadOperation(const std::string& bin_name);
		
	};
	
	class WriteOperation final : public BinOperation
	{
	public:
		
		WriteOperation() = delete;
		
		explicit WriteOperation(const DoubleBin& bin);
		explicit WriteOperation(const Int64Bin& bin);
		explicit WriteOperation(const StringBin& bin);
		
	};
	
	class IncrementOperation final : public BinOperation
	{
	public:
		
		IncrementOperation() = delete;
		
		explicit IncrementOperation(const DoubleBin& bin);
		explicit IncrementOperation(const Int64Bin& bin);
		
	};
	
	class AppendOperation final : public BinOperation
	{
	public:
		
		AppendOperation() = delete;
		
		explicit AppendOperation(const StringBin& bin);
		
	};
	
	class PrependOperation final : public BinOperation
	{
	public:
		
		PrependOperation() = delete;
		
		explicit PrependOperation(const StringBin& bin);
		
	};
	
	class TouchOperation final : public RecordOperation
	{
	public:
		
		TouchOperation();
		
	};

} // asw

#endif //AEROSPIKEOPERATION_HPP
