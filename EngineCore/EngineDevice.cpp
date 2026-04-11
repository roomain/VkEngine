#include "pch.h"
#include "EngineDevice.h"
#include "vmaMemoryCallbacks.h"
#include "vmaMemoryAllocator.h"

void EngineDevice::createMemoryAllocator(DeviceContext& a_ctx)
{
	static const VkAllocationCallbacks cpuAllocationCallbacks = {
		nullptr, // pUserData
		&vmaCpuAllocation, // pfnAllocation
		&vmaCpuReallocation, // pfnReallocation
		&vmaCpuFree // pfnFree
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
		.physicalDevice = a_ctx.m_vkPhysDevice,
		.device = a_ctx.m_vkDevice,
		.preferredLargeHeapBlockSize = 0,
		.pAllocationCallbacks = &cpuAllocationCallbacks,
		.pDeviceMemoryCallbacks = nullptr,
		.pHeapSizeLimit = 0,
		.pVulkanFunctions = &vulkanFunctions,
		.instance = a_ctx.m_vkInstance,
		.vulkanApiVersion = apiVersion
	};

	VK_CHECK_EXCEPT(vmaCreateAllocator(&vmaInfo, &a_ctx.m_memAllocator))
}

EngineDevice::EngineDevice(const uint32_t a_devIndex, const DeviceContext& a_ctx) :
	m_deviceIndex{ a_devIndex }, m_deviceCtx{ a_ctx }, m_queuesMng{ a_ctx.m_vkDevice, a_ctx.m_vkPhysDevice }
{
	if (m_deviceCtx.m_memAllocator == VK_NULL_HANDLE)
		createMemoryAllocator(m_deviceCtx);
}