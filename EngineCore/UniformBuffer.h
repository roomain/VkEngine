#pragma once
/***********************************************
* @headerfile Uniform.h
* @date 20 / 09 / 2026
* @author Roomain
************************************************/
#include "EngineLog.h"
#include "Buffer.h"
#include "vk_mem_alloc.h"
#include "VulkanBufferInitializers.h"

template<typename Type>
class UniformBuffer : public Buffer
{
	friend class EngineDevice;
private:
	explicit UniformBuffer(const DeviceContext& a_ctxt);

	void internalWrite(const Type& a_data)
	{
		if (m_bufferSize < sizeof(Type))
			releaseBuffer(m_buffer);

		if (m_buffer.buffer == VK_NULL_HANDLE)
		{
			BufferCreateInfoParameters params{
				.flags = 0,
				.size = sizeof(Type),
				.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.familyIndex = nullptr
			};

			auto bufferCreateInfoCI = bufferCreateInfo(params);

			m_bufferSize = sizeof(Type);
			VmaAllocationCreateInfo allocCreateInfo{};
			allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

			VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.memAllocator, &bufferCreateInfoCI, &allocCreateInfo, &m_buffer.buffer, &m_buffer.allocation, nullptr));
		}
		VK_CHECK_LOG(vmaCopyMemoryToAllocation(m_devCtx.memAllocator, &a_data, m_buffer.allocation, 0, m_bufferSize))
	}

	void internalRead(Type& a_data)const
	{
		VK_CHECK_LOG(vmaCopyAllocationToMemory(m_devCtx.memAllocator, m_buffer.allocation, 0, &a_data, sizeof(Type)))
	}

public:
	void writeData(const Type& a_data)
	{
		internalWrite(a_data);
	}

	void readData(Type& a_data)const
	{
		internalRead(a_data);
	}
};