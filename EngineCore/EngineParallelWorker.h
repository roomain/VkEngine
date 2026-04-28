#pragma once
/***********************************************
* @headerfile EngineParallelWorker.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include "notCopiable.h"
#include "DeviceContext.h"
#include "QueuesManager.h"
#include "ScopedLink.h"

using ContextFun = std::function<void(const DeviceContext&, VkQueue&)>;

/*@brief extension used to manage used queues in command threads*/
template<size_t Size>
class QueuePoolArray : public ManagedQueueArray<Size>
{
private:
	std::array<bool, Size> m_inUse;	/*!< use flag*/

public:
	QueuePoolArray() = delete;
	NOT_COPIABLE(QueuePoolArray)
	explicit QueuePoolArray(ManagedQueueArray<Size>&& a_managed) : ManagedQueueArray<Size>(a_managed)
	{
		for (auto& inUse : m_inUse)
			inUse = false;
	}

	std::optional<ScopedLink<VkQueue>> next()
	{
		int index = 0;
		for (auto& isUsed : m_inUse)
		{
            if (!isUsed)
				return std::make_optional<ScopedLink<VkQueue>>(this->m_queues[index], isUsed);

			++index;
		}
		return std::optional<ScopedLink<VkQueue>>();
	}
};


/*@brief Use to do vulkan work in parallel
* using common device and memory
*/
template<size_t Size>
class EngineParallelWorker
{
	friend class EngineDevice;

private:
	DeviceContext m_DeviceCtx;				/*!< vulkan device context*/
	QueuePoolArray<Size> m_queue;			/*!< working queues*/
	boost::asio::thread_pool m_workerPool;	/*!< thread pool*/

	explicit EngineParallelWorker(const DeviceContext& a_ctx, ManagedQueueArray<Size>&& a_data) :
		m_DeviceCtx{ a_ctx }, m_queue { a_data }, m_workerPool(Size)
	{
		// nothing to do
	}

public:
	EngineParallelWorker() = delete;
	NOT_COPIABLE(EngineParallelWorker)


	void postTask(ContextFun& a_fun)
	{
		boost::asio::post(m_workerPool, [this]()
			{
				if (auto optQueue = m_queue.next(); optQueue.has_value())
					a_fun(m_DeviceCtx, optQueue.get());
			});
	}

	void waitForAllTasks()
	{
		m_workerPool.wait();
	}
};
