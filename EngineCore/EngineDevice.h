#pragma once
/***********************************************
* @headerfile EngineDevice.h
* @date 13 / 03 / 2026
* @author Roomain
************************************************/
#include "DeviceContext.h"
#include "EngineQueueManager.h"
#include "notCopiable.h"
#include "EngineParallelWorker.h"
#include "UniformBuffer.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

class Buffer;
class StagingBuffer;
class IndexBuffer;
class VertexBuffer;
class StorageBuffer;

struct DeviceConfiguration;

/*@brief Base class of device created by EngineApplication*/
class ENGINECORE_EXPORT EngineDevice
{
	friend class EngineApplication;
protected:
	uint32_t m_deviceIndex;			/*!< device index in instance*/
	DeviceContext m_deviceCtx;		/*!< device context: device handle + instance handle + memory */
	EngineQueueManager m_queuesMng;	/*!< queue manager */

	/*@brief create memory allocator for DeviceContext*/
	static void createMemoryAllocator(DeviceContext& a_ctx);

	explicit EngineDevice(const DeviceConfiguration& a_parameters, const DeviceContext& a_ctx);

public:
	EngineDevice() = delete;
	NOT_COPIABLE(EngineDevice)

	[[nodiscard]] const DeviceContext& deviceContext()const { return m_deviceCtx; }
	[[nodiscard]] constexpr uint32_t deviceIndex()const { return m_deviceIndex; }
	[[nodiscard]] inline EngineQueueManager& queueManager() { return m_queuesMng; }

	template<size_t Size>
	[[nodiscard]] EngineParallelWorkerPtr<Size> createParallelWorker(const VkQueueFlags a_flag)
	{
		return EngineParallelWorkerPtr<Size>(new EngineParallelWorker<Size>(m_deviceCtx, std::move(m_queuesMng.createArray<Size>(a_flag))));
	}

	[[nodiscard]] std::shared_ptr<StagingBuffer> createStagingBuffer()const;
	[[nodiscard]] std::shared_ptr<IndexBuffer> createIndexBuffer()const;
	[[nodiscard]] std::shared_ptr<VertexBuffer> createVertexBuffer()const;

	template<typename Type>
	[[nodiscard]] std::shared_ptr<UniformBuffer<Type>> createUniformBuffer()const
	{
		// use new operator because ctor is private
		return std::shared_ptr<UniformBuffer<Type>>(new UniformBuffer<Type>(m_deviceCtx));
	}

	[[nodiscard]] std::shared_ptr<StorageBuffer> createStorageBuffer()const;
};
#pragma warning(pop)
