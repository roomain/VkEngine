#pragma once
/***********************************************
* @headerfile ManagedCommandBuffer.h
* @date 26 / 09 / 2026
* @author Roomain
************************************************/
#include "notCopiable.h"
#include <vulkan/vulkan.hpp>
#include "enginecore_globals.h"

class ENGINECORE_EXPORT ManagedCommandBuffer
{
	friend class QueueManager;
private:
	VkCommandPool m_commandPool = VK_NULL_HANDLE;	/*!< command pool*/
	VkDevice m_logicalDevice = VK_NULL_HANDLE;		/*!< logical device*/
	VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;	/*!< command buffer*/

	ManagedCommandBuffer(VkDevice a_device, VkCommandPool a_cmdPool, const VkCommandBufferLevel a_level);

public:
	ManagedCommandBuffer() = delete;
	NOT_COPIABLE(ManagedCommandBuffer)
	virtual ~ManagedCommandBuffer();
	inline [[nodiscard]] VkCommandBuffer& commandBuffer() { return m_cmdBuffer; }
};
