#ifndef AEROSPIKETRANSACTION_HPP
#define AEROSPIKETRANSACTION_HPP

#include <iostream>
#include <vector>

#include "AerospikeOperation.hpp"
#include "AerospikeCallback.hpp"
#include "AerospikeKey.hpp"
#include "AerospikeTransactionPolicy.hpp"

namespace asw
{
	class Transaction
	{
	public:
		
		Transaction() = delete;
		virtual ~Transaction() = default;
		
		static std::shared_ptr<Transaction> NewTransaction(const Key& key, const TransactionPolicy& policy, const BaseCallback& callback);
		
		const std::vector<BaseOperation>& GetOperations() const;
		void AddOperation(const BaseOperation& operation);
		void Delete();
	
	public:
		
		Key key;
		TransactionPolicy policy;
		BaseCallback callback;
	
	protected:
		
		Transaction(Key key, TransactionPolicy policy, BaseCallback callback);
	
	private:
		
		std::shared_ptr<Transaction> m_self;
		std::vector<BaseOperation> m_operations;
	};
	
	struct Transaction_ final : public Transaction
	{
	public:
		Transaction_() = delete;
		Transaction_(const Key& key, const TransactionPolicy& policy, const BaseCallback& callback);
	};
	
} // asw

#endif //AEROSPIKETRANSACTION_HPP
