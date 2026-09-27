#include "pch.h"
#include <format>
#include <algorithm>
#include "QueueManager.h"

void QueueManager::getQueue(const uint32_t a_family, Queue& a_queue)
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

void QueueManager::releaseQueue(const uint32_t a_familyIndex, const uint32_t a_queueIndex)
{
	if (auto iter = std::ranges::find(m_stats[a_familyIndex].usedQueues, a_queueIndex); iter != m_stats[a_familyIndex].usedQueues.cend())
		m_stats[a_familyIndex].usedQueues.erase(iter);
}

void QueueManager::releaseQueueList(const uint32_t a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices)
{
	for (size_t index = 0; index < a_size; ++index)
		releaseQueue(a_familyIndex, a_queueIndices[index]);
}

QueueManager::QueueManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues) :
	m_logicalDevice{ a_dev }
{
	for (const auto& prop : a_usedQueues)
	{
		auto [iter, valid] = m_stats.emplace(prop.familyIndex, QueueFamilyStatistics{ prop.flags, prop.queueCount });
		auto cmdPoolInfo = initCommandPoolCreateInfo(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, prop.familyIndex);
		VK_CHECK_LOG(vkCreateCommandPool(a_dev, &cmdPoolInfo, nullptr, &(iter->second.commandPool)))
	}
}

std::vector<uint32_t> QueueManager::findFamilies(const VkQueueFlags a_flags)
{
	std::vector<uint32_t> families;	
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flags) == a_flags)
			families.emplace_back(family);
	}
	return families;
}

ManagedQueue QueueManager::createQueue(const VkQueueFlags a_flag)
{
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flag) == a_flag &&
			static_cast<uint32_t>(stat.usedQueues.size()) <  stat.queueCount)
		{			
			Queue queue;
			getQueue(family, queue);
			return ManagedQueue(family, std::move(queue), std::bind_front(&QueueManager::releaseQueue, this));
		}
	}
	throw ManageException(std::source_location::current(), "Not enough queue");
}

ManagedCommandBuffer QueueManager::createCommandBuffer(const uint32_t a_family, const VkCommandBufferLevel a_level)
{
	auto iter = m_stats.find(a_family);
	if (iter == m_stats.cend())
		throw ManageException(std::source_location::current(), std::format("No family {}", a_family).c_str());

	return ManagedCommandBuffer(m_logicalDevice, iter->second.commandPool, a_level);
}
