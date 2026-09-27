#include "pch.h"
#include "ManagedCommandBuffer.h"

ManagedCommandBuffer::ManagedCommandBuffer(VkDevice a_device, VkCommandPool a_cmdPool, const VkCommandBufferLevel a_level) : m_commandPool{ a_cmdPool }, m_logicalDevice { a_device }
{
	auto allocation = initAllocCommandBufferInfo(m_commandPool, a_level, 1);
	VK_CHECK_LOG(vkAllocateCommandBuffers(m_logicalDevice, &allocation, &m_cmdBuffer))
}

ManagedCommandBuffer::~ManagedCommandBuffer()
{
	vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &m_cmdBuffer);
}