#include "AerospikeOperation.hpp"

namespace asw
{
	void BaseOperation::Apply(as_operations* operations) const
	{
		m_apply(operations);
	}
	
	
	BinOperation::BinOperation(const BaseBin& bin) : m_bin{bin}
	{
	
	}
	
	const BaseBin& BinOperation::Bin() const
	{
		return m_bin;
	}

	
	ReadOperation::ReadOperation(const std::string& bin_name) : BinOperation(BaseBin(bin_name))
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_read(operations, bin_name.c_str());
		};
		
	}

	
	WriteOperation::WriteOperation(const DoubleBin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_write_double(operations, bin.Name().c_str(), bin.Value());
		};
		
	}
	
	WriteOperation::WriteOperation(const Int64Bin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_write_int64(operations, bin.Name().c_str(), bin.Value());
		};
		
	}
	
	WriteOperation::WriteOperation(const StringBin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_write_str(operations, bin.Name().c_str(), bin.Value().c_str());
		};
		
	}

	
	IncrementOperation::IncrementOperation(const DoubleBin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_incr_double(operations, bin.Name().c_str(), bin.Value());
		};
		
	}
	
	IncrementOperation::IncrementOperation(const Int64Bin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_incr(operations, bin.Name().c_str(), bin.Value());
		};
		
	}

	
	AppendOperation::AppendOperation(const StringBin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_append_str(operations, bin.Name().c_str(), bin.Value().c_str());
		};
		
	}

	
	PrependOperation::PrependOperation(const StringBin& bin) : BinOperation(bin)
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_prepend_str(operations, bin.Name().c_str(), bin.Value().c_str());
		};
		
	}

	
	TouchOperation::TouchOperation()
	{
		m_apply = [=](as_operations* operations)
		{
			as_operations_add_touch(operations);
		};
		
	}

} // asw
