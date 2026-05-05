#include "pch.h"
#include "EngineBuffer.h"

void EngineDynamicBuffer::releaseBuffer()
{
    if (m_buffer)
    {
        vmaDestroyBuffer(m_devCtx.m_memAllocator, m_buffer, m_allocation);
        m_buffer = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
    }
}


EngineDynamicBuffer::~EngineDynamicBuffer()
{
    if (m_buffer)
        vmaDestroyBuffer(m_devCtx.m_memAllocator, m_buffer, m_allocation);
}
