#include "pch.h"
#include "QueuesManager.h"

void QueuesManager::release(const int a_familyIndex, const size_t& a_count)
{
	m_stats[a_familyIndex].inUse -= static_cast<uint32_t>(a_count);
}

QueuesManager::QueuesManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev) :
	m_logicalDevice{ a_dev }
{
	std::vector<VkQueueFamilyProperties> queueFamilies;
	getQueueFamiliesCapabilities(a_physDev, queueFamilies);
	int index = 0;
	for (const auto& prop : queueFamilies)
	{
		m_stats.emplace(index, QueueFamilyStatistics{ prop });
		++index;
	}
}

ManagedQueue QueuesManager::createQueue(VkQueueFlags a_flag)
{
	for (auto& [family, stat] : m_stats)
	{
		if ((stat.queueFlags & a_flag) == a_flag &&
			(stat.queueCount - stat.inUse) >= 1)
		{
			stat.inUse++;
			VkQueue queue;
			vkGetDeviceQueue(m_logicalDevice, family, 1, &queue);
			return ManagedQueue(family, queue, std::bind_front(&QueuesManager::release, this));
		}
	}
	throw EngineManageException(std::source_location::current(), "Not enough queue");
}