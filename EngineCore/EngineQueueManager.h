#pragma once
/***********************************************
* @headerfile EngineQueueManager.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <array>
#include <functional>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "notCopiable.h"
#include "EngineExceptions.h"
#include "EngineManagedQueue.h"
#include "EngineManagedQueueArray.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

struct QueueConfiguration;


/*@brief provide management of vulkan device queue*/
/*each queue in ManagedQueue/ManagedQueueArray can be release and reused*/
class ENGINECORE_EXPORT EngineQueueManager
{
private:
	struct QueueFamilyStatistics
	{
		VkQueueFlags queueFlags;			/*!< queue family flags*/
		uint32_t queueCount;				/*!< queue count*/
		std::vector<uint32_t> usedQueues;	/*!< index of queue in use*/
	};

	VkDevice m_logicalDevice;								/*!< logical device*/
	std::unordered_map<int, QueueFamilyStatistics> m_stats; /*!< statistics per family*/

	void releaseQueue(const int a_familyIndex, const uint32_t a_queueIndex);
	void releaseQueueList(const int a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices);
	void getQueue(const int a_family, EngineQueue& a_queue);

	template<size_t Size>
	void getQueues(const int a_family, std::array<EngineQueue, Size>& a_queues)
	{
		for (auto& queue : a_queues)
			getQueue(a_family, queue);
	}

public:
	// to rework ctor to create stats from device creation
	EngineQueueManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues);
	EngineQueueManager() = delete;
	NOT_COPIABLE(EngineQueueManager);

	[[nodiscard]] EngineManagedQueue createQueue(VkQueueFlags a_flags);

	template<size_t Size>
	[[nodiscard]] EngineManagedQueueArray<Size>&& createArray(const VkQueueFlags a_flag)
	{
		for (const auto& [family, stat] : m_stats)
		{
			if ((stat.queueFlags & a_flag) == a_flag &&
				(stat.queueCount - static_cast<uint32_t>(stat.usedQueues.size())) >= static_cast<uint32_t>(Size))
			{
				std::array<EngineQueue, Size> queuesArray;
				getQueues(family, queuesArray);
				return std::move(EngineManagedQueueArray<Size>(family, std::move(queuesArray), std::bind_front(&EngineQueueManager::releaseQueueList, this)));
			}
		}
		throw EngineManageException(std::source_location::current(), "Not enough queue");
	}
};

#pragma warning(pop)