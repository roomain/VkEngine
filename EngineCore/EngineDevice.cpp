#include "pch.h"
#include "EngineDevice.h"
#include "StagingBuffer.h"
#include "GPUBuffers.h"
#include "vmaMemoryCallbacks.h"
#include "vmaMemoryAllocator.h"

void EngineDevice::createMemoryAllocator(DeviceContext& a_ctx)
{
	static const VkAllocationCallbacks cpuAllocationCallbacks = {
		nullptr,				// pUserData
		&vmaCpuAllocation,		// pfnAllocation
		&vmaCpuReallocation,	// pfnReallocation
		&vmaCpuFree				// pfnFree
	};

	static VmaVulkanFunctions vulkanFunctions
	{
		.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = vkGetDeviceProcAddr
	};

	uint32_t apiVersion = VK_API_VERSION_1_0;
	vkEnumerateInstanceVersion(&apiVersion);

	VmaAllocatorCreateInfo vmaInfo
	{
		.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_AMD_DEVICE_COHERENT_MEMORY_BIT |
		VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT
		/* | VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT*/,
		.physicalDevice = a_ctx.vkPhysDevice,
		.device = a_ctx.vkDevice,
		.preferredLargeHeapBlockSize = 0,
		.pAllocationCallbacks = &cpuAllocationCallbacks,
		.pDeviceMemoryCallbacks = nullptr,
		.pHeapSizeLimit = 0,
		.pVulkanFunctions = &vulkanFunctions,
		.instance = a_ctx.vkInstance,
		.vulkanApiVersion = apiVersion
	};

	VK_CHECK_EXCEPT(vmaCreateAllocator(&vmaInfo, &a_ctx.memAllocator))
}

EngineDevice::EngineDevice(const DeviceConfiguration& a_parameters, const DeviceContext& a_ctx) :
	m_deviceIndex{ a_parameters.deviceIndex }, m_deviceCtx{ a_ctx }, 
	m_queuesMng{ a_ctx.vkDevice, a_ctx.vkPhysDevice, a_parameters.queues }
{
	if (m_deviceCtx.memAllocator == VK_NULL_HANDLE)
		createMemoryAllocator(m_deviceCtx);
}


std::shared_ptr<StagingBuffer> EngineDevice::createStagingBuffer()const
{
	// use new operator because ctor is private
	return std::shared_ptr<StagingBuffer>(new StagingBuffer(m_deviceCtx));
}

std::shared_ptr<IndexBuffer> EngineDevice::createIndexBuffer()const
{
	// use new operator because ctor is private
	return std::shared_ptr<IndexBuffer>(new IndexBuffer(m_deviceCtx));
}

std::shared_ptr<VertexBuffer> EngineDevice::createVertexBuffer()const
{
	// use new operator because ctor is private
	return std::shared_ptr<VertexBuffer>(new VertexBuffer(m_deviceCtx));
}


std::shared_ptr<StorageBuffer> EngineDevice::createStorageBuffer()const
{
	// use new operator because ctor is private
	return std::shared_ptr<StorageBuffer>(new StorageBuffer(m_deviceCtx));
}