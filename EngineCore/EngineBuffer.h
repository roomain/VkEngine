#pragma once
/***********************************************
* @headerfile EngineBuffer.h
* @date 03 / 06 / 2026
* @author Roomain
************************************************/
#include "DeviceContext.h"
#include "EngineExceptions.h"

class EngineDynamicBuffer
{
private:
	DeviceContext m_devCtx; /*!< device context*/
	VmaAllocation m_allocation = VK_NULL_HANDLE;
	VkBufferCreateInfo m_bufferCreateInfo;  /*!< buffer create information*/
	VkBuffer m_buffer = VK_NULL_HANDLE;     /*!< buffer handle*/
	VkDeviceSize m_activeSize = 0;

	void releaseBuffer();
	explicit EngineDynamicBuffer(const DeviceContext& a_ctxt) : m_devCtx{ a_ctxt } {}
public:
	~EngineDynamicBuffer();
	[[nodiscard]] constexpr VkBuffer buffer()const { return m_buffer; }
	[[nodiscard]] VkDeviceSize allocationSize()const { return m_bufferCreateInfo.size; }
	[[nodiscard]] VkDeviceSize bufferSize()const { return m_activeSize; }

	template<typename Type>
	void setData(Type* a_buffer, const size_t& a_bufferByteSize)
	{
		if (m_bufferCreateInfo.size < a_bufferByteSize)
			releaseBuffer();

		if (m_buffer == VK_NULL_HANDLE)
		{
			m_bufferCreateInfo.size = a_bufferByteSize;
			VmaAllocationCreateInfo allocCreateInfo = {};
			allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
			allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

			vmaCreateBuffer(m_devCtx.m_memAllocator, &m_bufferCreateInfo, &allocCreateInfo, &m_buffer, &m_allocation, nullptr);
		}
		m_activeSize = a_bufferByteSize;
		vmaCopyMemoryToAllocation(m_devCtx.m_memAllocator, a_buffer, m_allocation, 0, a_bufferByteSize);
	}
};


