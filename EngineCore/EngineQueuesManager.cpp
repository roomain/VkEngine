#include "pch.h"
#include <algorithm>
#include "EngineQueueManager.h"

void EngineQueueManager::getQueue(const int a_family, EngineQueue& a_queue)
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

void EngineQueueManager::releaseQueue(const int a_familyIndex, const uint32_t a_queueIndex)
{
	if (auto iter = std::ranges::find(m_stats[a_familyIndex].usedQueues, a_queueIndex); iter != m_stats[a_familyIndex].usedQueues.cend())
		m_stats[a_familyIndex].usedQueues.erase(iter);
}

void EngineQueueManager::releaseQueueList(const int a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices)
{
	for (size_t index = 0; index < a_size; ++index)
		releaseQueue(a_familyIndex, a_queueIndices[index]);
}

EngineQueueManager::EngineQueueManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues) :
	m_logicalDevice{ a_dev }
{
	for (const auto& prop : a_usedQueues)
		m_stats.emplace(prop.familyIndex, QueueFamilyStatistics{ prop.flags, prop.queueCount });
}

EngineManagedQueue EngineQueueManager::createQueue(VkQueueFlags a_flag)
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