#include "pch.h"
#include <algorithm>
#include "EngineQueueManager.h"

void EngineQueueManager::getQueue(const uint32_t a_family, EngineQueue& a_queue)
{
	for (uint32_t index = 0; index < m_stats[a_family].queueCount; ++index)
	{
		if (std::ranges::none_of(m_stats[a_family].usedQueues, [index](const auto queueIndex)
			{
				return queueIndex == index;
			}))
		{
			a_queue.queueIndex = index;
			m_stats[a_family].usedQueues.emplace_back(index);
			vkGetDeviceQueue(m_logicalDevice, a_family, index, &a_queue.queue);
			break;
		}
	}
}

void EngineQueueManager::releaseQueue(const uint32_t a_familyIndex, const uint32_t a_queueIndex)
{
	if (auto iter = std::ranges::find(m_stats[a_familyIndex].usedQueues, a_queueIndex); iter != m_stats[a_familyIndex].usedQueues.cend())
		m_stats[a_familyIndex].usedQueues.erase(iter);
}

void EngineQueueManager::releaseQueueList(const uint32_t a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices)
{
	for (size_t index = 0; index < a_size; ++index)
		releaseQueue(a_familyIndex, a_queueIndices[index]);
}

EngineQueueManager::EngineQueueManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues) :
	m_logicalDevice{ a_dev }
{
	for (const auto& prop : a_usedQueues)
	{
		auto [iter, valid] = m_stats.emplace(prop.familyIndex, QueueFamilyStatistics{ prop.flags, prop.queueCount });
		auto cmdPoolInfo = initCommandPoolCreateInfo(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, prop.familyIndex);
		VK_CHECK_LOG(vkCreateCommandPool(a_dev, &cmdPoolInfo, nullptr, &(iter->second.commandPool)))
	}
}

std::vector<uint32_t> EngineQueueManager::findFamilies(const VkQueueFlags a_flags)
{
	std::vector<uint32_t> families;	
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flags) == a_flags)
			families.emplace_back(family);
	}
	return families;
}

EngineManagedQueue EngineQueueManager::createQueue(const VkQueueFlags a_flag)
{
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flag) == a_flag &&
			static_cast<uint32_t>(stat.usedQueues.size()) <  stat.queueCount)
		{			
			EngineQueue queue;
			getQueue(family, queue);
			return EngineManagedQueue(family, std::move(queue), std::bind_front(&EngineQueueManager::releaseQueue, this));
		}
	}
	throw EngineManageException(std::source_location::current(), "Not enough queue");
}

VkCommandBuffer EngineQueueManager::createCommandBuffer(const uint32_t a_family, const VkCommandBufferLevel a_level)
{
	VkCommandBuffer cmdBuff = VK_NULL_HANDLE; 
	auto iter = m_stats.find(a_family);
	if ( iter == m_stats.cend())
		return cmdBuff;

	auto allocation = initAllocCommandBufferInfo(iter->second.commandPool, a_level, 1);
	VK_CHECK_LOG(vkAllocateCommandBuffers(m_logicalDevice, &allocation, &cmdBuff))
	return cmdBuff;
}

void EngineQueueManager::freeCommandBuffer(const uint32_t a_family, VkCommandBuffer& a_cmdBuffer)
{
	vkFreeCommandBuffers(m_logicalDevice, m_stats[a_family].commandPool, 1, &a_cmdBuffer);
	a_cmdBuffer = VK_NULL_HANDLE;
}