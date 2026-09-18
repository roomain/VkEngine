#include "pch.h"
#include "GPUBuffers.h"

EngineIndexBuffer::EngineIndexBuffer(const DeviceContext& a_ctxt) : EngineBuffer(a_ctxt){}

void EngineIndexBuffer::allocate(const size_t& a_size)
{
	if (m_bufferSize < a_size)
		releaseBuffer(m_buffer);

	if (m_buffer.buffer == VK_NULL_HANDLE)
	{
		BufferCreateInfoParameters params{
			.flags = 0,
			.size = a_size,
			.usage = VMA_MEMORY_USAGE_GPU_ONLY,
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
}

void EngineIndexBuffer::bind(VkCommandBuffer& a_cmdBuffer)
{
	vkCmdBindIndexBuffer(a_cmdBuffer, m_buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
}


EngineVertexBuffer::EngineVertexBuffer(const DeviceContext& a_ctxt) : EngineBuffer(a_ctxt) {}

void EngineVertexBuffer::allocate(const size_t& a_size)
{
	if (m_bufferSize < a_size)
		releaseBuffer(m_buffer);

	if (m_buffer.buffer == VK_NULL_HANDLE)
	{
		BufferCreateInfoParameters params{
			.flags = 0,
			.size = a_size,
			.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.familyIndex = nullptr
		};

		auto bufferCreateInfoCI = bufferCreateInfo(params);

		m_bufferSize = a_size;
		VmaAllocationCreateInfo allocCreateInfo{};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.memAllocator, &bufferCreateInfoCI, &allocCreateInfo, &m_buffer.buffer, &m_buffer.allocation, nullptr));
	}
}

void EngineVertexBuffer::bind(VkCommandBuffer& a_cmdBuffer, uint32_t a_bind)
{
	vkCmdBindVertexBuffers(a_cmdBuffer, a_bind, 1, &m_buffer.buffer, &m_bufferSize);
}


EngineUniformBuffer::EngineUniformBuffer(const DeviceContext& a_ctxt) : EngineBuffer(a_ctxt) {}

void EngineUniformBuffer::allocate(const size_t& a_size)
{
	if (m_bufferSize < a_size)
		releaseBuffer(m_buffer);

	if (m_buffer.buffer == VK_NULL_HANDLE)
	{
		BufferCreateInfoParameters params{
			.flags = 0,
			.size = a_size,
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
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
}

EngineStorageBuffer::EngineStorageBuffer(const DeviceContext& a_ctxt) : EngineBuffer(a_ctxt) {}

void EngineStorageBuffer::allocate(const size_t& a_size)
{
	if (m_bufferSize < a_size)
		releaseBuffer(m_buffer);

	if (m_buffer.buffer == VK_NULL_HANDLE)
	{
		BufferCreateInfoParameters params{
			.flags = 0,
			.size = a_size,
			.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.familyIndex = nullptr
		};

		auto bufferCreateInfoCI = bufferCreateInfo(params);

		m_bufferSize = a_size;
		VmaAllocationCreateInfo allocCreateInfo{};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VK_CHECK_LOG(vmaCreateBuffer(m_devCtx.memAllocator, &bufferCreateInfoCI, &allocCreateInfo, &m_buffer.buffer, &m_buffer.allocation, nullptr));
	}
}
