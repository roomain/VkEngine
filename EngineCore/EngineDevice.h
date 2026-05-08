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

struct DeviceConfiguration;

/*@brief Base class of device created by EngineApplication*/
class EngineDevice
{
	friend class EngineApplication;
protected:
	uint32_t m_deviceIndex;		/*!< device index in instance*/
	DeviceContext m_deviceCtx;	/*!< device context: device handle + instance handle + memory */
	QueuesManager m_queuesMng;	/*!< queue manager */

	/*@brief create memory allocator for DeviceContext*/
	static void createMemoryAllocator(DeviceContext& a_ctx);

	explicit EngineDevice(const DeviceConfiguration& a_parameters, const DeviceContext& a_ctx);

public:
	EngineDevice() = delete;
	NOT_COPIABLE(EngineDevice)

	[[nodiscard]] const DeviceContext& deviceContext()const { return m_deviceCtx; }
	[[nodiscard]] constexpr uint32_t deviceIndex()const { return m_deviceIndex; }

	template<size_t Size>
	EngineParallelWorker<Size> createParallelWorker(VkQueueFlags a_flag)
	{
		return EngineParallelWorker<Size>(m_deviceCtx, m_queuesMng.createArray(a_flag));
	}

};
