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
