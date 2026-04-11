#pragma once
/***********************************************
* @headerfile EngineDevice.h
* @date 13 / 03 / 2026
* @author Roomain
************************************************/
#include "DeviceContext.h"
#include "QueuesManager.h"
#include "notCopiable.h"
#include "EngineParallelWorker.h"

/*@brief Base class of device created by EngineApplication*/
class EngineDevice
{
	friend class EngineApplication;
private:
	uint32_t m_deviceIndex;		/*!< device index in instance*/
	DeviceContext m_deviceCtx;	/*!< device context: device handle + instance handle + memory */
	QueuesManager m_queuesMng;	/*!< queue manager */

	/*@brief create memory allocator for DeviceContext*/
	static void createMemoryAllocator(DeviceContext& a_ctx);

	explicit EngineDevice(const uint32_t a_devIndex, const DeviceContext& a_ctx);

public:
	EngineDevice() = delete;
	NOT_COPIABLE(EngineDevice)

	const DeviceContext& deviceContext()const { return m_deviceCtx; }

	template<size_t Size>
	EngineParallelWorker<Size> createParallelWorker(VkQueueFlags a_flag)
	{
		return EngineParallelWorker<Size>(m_deviceCtx, m_queuesMng.createArray(a_flag));
	}

};
