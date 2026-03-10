#pragma once
/***********************************************
* @headerfile VkParallelWorker.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <memory>
#include <functional>
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
#include <boost/asio/thread_pool.hpp>
#include "ArrayPool.h"
#include "notCopiable.h"
#include "DeviceContext.h"

using ContextFun = std::function<void(const DeviceContext&, VkQueue&)>;


/*@brief Use to do vulkan work in parallel
* using common device and memory
*/
template<typename Size>
class VkParallelWorker
{
	friend class Engine;

protected:
	DeviceContext m_DeviceCtx;				/*!< vulkan device context*/
	ArrayPool<VkQueue, Size> m_queue;		/*!< working queues*/
	boost::asio::thread_pool m_workerPool;	/*!< thread pool*/

public:
	VkParallelWorker() = delete;
	NOT_COPIABLE(VkParallelWorker)

	explicit VkParallelWorker(const DeviceContext& a_ctx, std::array<VkQueue, Size> a_data) :
		m_DeviceCtx{ a_ctx } m_queue{ a_data }, m_workerPool(Size)
	{
		// nothing to do
	}

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
