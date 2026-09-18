#include "pch.h"
#include "EngineStagingBuffer.h"

EngineStagingBuffer::EngineStagingBuffer(const DeviceContext& a_ctxt) : EngineBuffer(a_ctxt){}

void EngineStagingBuffer::internalWrite(const void* a_data, const size_t& a_size)
{
	if (m_bufferSize < a_size)
		releaseBuffer(m_buffer);

	if (m_buffer.buffer == VK_NULL_HANDLE)
	{
		BufferCreateInfoParameters params{
			.flags = 0,
			.size = a_size,
			.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.familyIndex = nullptr
		};

		auto bufferCreateInfoCI = bufferCreateInfo(params);

		m_bufferSize = a_size;
		VmaAllocationCreateInfo allocCreateInfo{};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.memAllocator, &bufferCreateInfoCI, &allocCreateInfo, &m_buffer.buffer, &m_buffer.allocation, nullptr));
	}
	VK_CHECK_LOG(vmaCopyMemoryToAllocation(m_devCtx.memAllocator, a_data, m_buffer.allocation, 0, a_size))
}

void EngineStagingBuffer::internalRead(void* a_data, const size_t& a_offset, const size_t& a_size)const
{
	VK_CHECK_LOG(vmaCopyAllocationToMemory(m_devCtx.memAllocator, m_buffer.allocation, a_offset, a_data, a_size))
}

void EngineStagingBuffer::copyTo(VkCommandBuffer& a_cmdBuffer, EngineBuffer& a_other)
{
	VkBufferCopy bufferCopy{
		.srcOffset = 0,
		.dstOffset = 0,
		.size = m_bufferSize
	};
	vkCmdCopyBuffer(a_cmdBuffer, m_buffer.buffer, a_other.buffer(), 1, &bufferCopy);
}