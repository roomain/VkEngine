#include "pch.h"
#include "EngineBuffer.h"

void EngineBuffer::releaseBuffer(VMABuffer& a_buffer)
{
	if (a_buffer.buffer != VK_NULL_HANDLE)
	{
		vmaDestroyBuffer(m_devCtx.memAllocator, a_buffer.buffer, a_buffer.allocation);
		a_buffer.buffer = VK_NULL_HANDLE;
		a_buffer.allocation = VK_NULL_HANDLE;
	}
}

void EngineBuffer::createBuffer(const VkBufferCreateInfo bufferInfo, VMABuffer& a_buffer)
{
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.memAllocator, &bufferInfo, &allocCreateInfo, &a_buffer.buffer, &a_buffer.allocation, nullptr));
}

EngineBuffer::EngineBuffer(const DeviceContext& a_ctxt) : m_devCtx{ a_ctxt }
{
}

EngineBuffer::~EngineBuffer()
{
	releaseBuffer(m_buffer);
}

