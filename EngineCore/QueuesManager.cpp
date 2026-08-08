#include "pch.h"
#include <algorithm>
#include "QueuesManager.h"

void QueuesManager::releaseQueue(const int a_familyIndex, const uint32_t a_queueIndex)
{
	if (auto iter = std::ranges::find(m_stats[a_familyIndex].usedQueues, a_queueIndex); iter != m_stats[a_familyIndex].usedQueues.cend())
		m_stats[a_familyIndex].usedQueues.erase(iter);
}

void QueuesManager::releaseQueueList(const int a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices)
{
	for (size_t index = 0; index < a_size; ++index)
		releaseQueue(a_familyIndex, a_queueIndices[index]);
}

QueuesManager::QueuesManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues) :
	m_logicalDevice{ a_dev }
{
	for (const auto& prop : a_usedQueues)
		m_stats.emplace(prop.familyIndex, QueueFamilyStatistics{ prop.flags, prop.queueCount });
}

ManagedQueue QueuesManager::createQueue(VkQueueFlags a_flag)
{
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flag) == a_flag &&
			static_cast<uint32_t>(stat.usedQueues.size()) <  stat.queueCount)
		{
			//stat.inUse++;
			VkQueue queue;
			// refaire structure contenant queue et index
			// todo find free index
			//vkGetDeviceQueue(m_logicalDevice, family, 1, &queue);
			return ManagedQueue(family, queue, std::bind_front(&QueuesManager::releaseQueue, this));
		}
	}
	throw EngineManageException(std::source_location::current(), "Not enough queue");
}