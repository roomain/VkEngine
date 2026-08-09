#include "pch.h"
#include "EngineBuffer.h"

EngineBuffer::EngineBuffer(const DeviceContext& a_ctxt) : m_devCtx{ a_ctxt }
{
	BufferCreateInfoParameters params{
			.flags = 0,
			.size = 0,
			.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.familyIndex = nullptr
	};
	m_bufferCreateInfo = bufferCreateInfo(params);
}

void EngineBuffer::releaseBuffer()
{
    if (m_buffer)
    {
        vmaDestroyBuffer(m_devCtx.m_memAllocator, m_buffer, m_allocation);
        m_buffer = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
    }
}


EngineBuffer::~EngineBuffer()
{
    if (m_buffer)
        vmaDestroyBuffer(m_devCtx.m_memAllocator, m_buffer, m_allocation);
}

void EngineBuffer::internalWrite(const void* a_data, const size_t& a_size)
{
	if (m_bufferCreateInfo.size < a_size)
		releaseBuffer();

	if (m_buffer == VK_NULL_HANDLE)
	{
		m_bufferCreateInfo.size = a_size;
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.m_memAllocator, &m_bufferCreateInfo, &allocCreateInfo, &m_buffer, &m_allocation, nullptr));
	}
	m_activeSize = a_size;
	VK_CHECK_LOG(vmaCopyMemoryToAllocation(m_devCtx.m_memAllocator, a_data, m_allocation, 0, a_size));
}

void EngineBuffer::internalRead(void* a_data, const size_t& a_offset, const size_t& a_size)const
{
	VK_CHECK_LOG(vmaCopyAllocationToMemory(m_devCtx.m_memAllocator, m_allocation, a_offset, a_data, a_size));
}