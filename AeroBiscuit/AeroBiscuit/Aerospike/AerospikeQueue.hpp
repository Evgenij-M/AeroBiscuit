#ifndef AEROSPIKEQUEUE_HPP
#define AEROSPIKEQUEUE_HPP

#include <queue> // std::queue
#include <mutex> // std::mutex, std::unique_lock, std::scoped_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function

namespace asw
{
	template<typename Data>
	class ConcurrentQueue final
	{
	private:
		
		std::queue<Data> m_queue;
		mutable std::mutex m_mutex;
		std::condition_variable m_condition_variable;
		std::unique_lock<std::mutex> external_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
	
	public:
		
		void Push(Data data)
		{
			std::unique_lock lock(m_mutex);
			
			m_queue.emplace(std::move(data));
			
			lock.unlock();
			
			m_condition_variable.notify_one();
		}
		
		bool Empty() const
		{
			std::scoped_lock lock(m_mutex);
			
			return m_queue.empty();
		}
		
		bool TryPop(Data &popped_value)
		{
			std::scoped_lock lock(m_mutex);
			
			if (m_queue.empty())
			{
				return false;
			}
			
			popped_value = m_queue.front();
			m_queue.pop();
			
			return true;
		}
		
		void WaitAndPop(Data &popped_value, const std::function<bool()> &urgent_exit)
		{
			std::unique_lock lock(m_mutex);
			
			m_condition_variable.wait(lock, [&]()
			{
				return not m_queue.empty() || urgent_exit();
			});
			
			if (urgent_exit())
			{
				popped_value = Data();
			}
			else
			{
				popped_value = m_queue.front();
				m_queue.pop();
			}
			
		}
		
		void NotifyQueue()
		{
			m_condition_variable.notify_one();
		}
		
		void LockQueueMutex()
		{
			external_lock.lock();
		}
		
		void UnlockQueueMutex()
		{
			external_lock.unlock();
		}
		
	};
	
} // asw

#endif //AEROSPIKEQUEUE_HPP
