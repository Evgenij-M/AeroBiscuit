#include "AerospikeTransaction.hpp"

namespace asw
{
	Transaction::Transaction(Key key, TransactionPolicy policy, BaseCallback callback) : key{std::move(key)}, policy{std::move(policy)}, callback{std::move(callback)}
	{
	
	}
	
	Transaction_::Transaction_(const Key& key, const TransactionPolicy& policy, const BaseCallback& callback) : Transaction(key, policy, callback)
	{
	
	}
	
	std::shared_ptr<Transaction> Transaction::NewTransaction(const Key& key, const TransactionPolicy& policy, const BaseCallback& callback)
	{
		const std::shared_ptr<Transaction_> transaction = std::make_shared<Transaction_>(key, policy, callback);
		transaction->m_self = transaction;
		
		return transaction;
	}
	
	const std::vector<BaseOperation>& Transaction::GetOperations() const
	{
		return m_operations;
	}
	
	void Transaction::AddOperation(const BaseOperation& operation)
	{
		m_operations.emplace_back(operation);
	}
	
	void Transaction::Delete()
	{
		m_self.reset();
	}

} // asw