#pragma once
/***********************************************
* @headerfile QueueManager.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "notCopiable.h"
#include "Exceptions.h"
#include "ManagedQueue.h"
#include "ManagedQueueArray.h"
#include "ManagedCommandBuffer.h"
#include "ManagedCommandBufferArray.h"
#include "VulkanInitializers.h"
#include "Log.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

struct QueueConfiguration;


/*@brief provide management of vulkan device queue*/
/*each queue in ManagedQueue/ManagedQueueArray can be release and reused*/
class ENGINECORE_EXPORT QueueManager
{
private:
	struct QueueFamilyStatistics
	{
		VkQueueFlags queueFlags;						/*!< queue family flags*/
		uint32_t queueCount;							/*!< queue count*/
		std::vector<uint32_t> usedQueues;				/*!< index of queue in use*/
		VkCommandPool commandPool = VK_NULL_HANDLE;		/*!< command pool*/
	};

	VkDevice m_logicalDevice;								/*!< logical device*/
	std::unordered_map<uint32_t, QueueFamilyStatistics> m_stats; /*!< statistics per family*/

	void releaseQueue(const uint32_t a_familyIndex, const uint32_t a_queueIndex);
	void releaseQueueList(const uint32_t a_familyIndex, const size_t& a_size, const uint32_t* a_queueIndices);
	void getQueue(const uint32_t a_family, Queue& a_queue);

	template<size_t Size>
	void getQueues(const uint32_t a_family, std::array<Queue, Size>& a_queues)
	{
		for (auto& queue : a_queues)
			getQueue(a_family, queue);
	}

public:
	// to rework ctor to create stats from device creation
	QueueManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues);
	QueueManager() = delete;
	NOT_COPIABLE(QueueManager);

	[[nodiscard]] std::vector<uint32_t> findFamilies(const VkQueueFlags a_flags);
	[[nodiscard]] ManagedQueue createQueue(const VkQueueFlags a_flags);
	[[nodiscard]] ManagedCommandBuffer createCommandBuffer(const uint32_t a_family, const VkCommandBufferLevel a_level);
	template<size_t Size>
	[[nodiscard]] ManagedCommandBufferArray<Size> createCommandBuffers(const uint32_t a_family, const VkCommandBufferLevel a_level)
	{
		auto iter = m_stats.find(a_family);
		if (iter == m_stats.cend())
			throw ManageException(std::source_location::current(), "No family {}", a_family);

		return ManagedCommandBufferArray<Size>(m_logicalDevice, iter->second.commandPool, a_level);
	}

	template<size_t Size>
	[[nodiscard]] ManagedQueueArray<Size> createArray(const VkQueueFlags a_flag)
	{
		for (const auto& [family, stat] : m_stats)
		{
			if ((stat.queueFlags & a_flag) == a_flag &&
				(stat.queueCount - static_cast<uint32_t>(stat.usedQueues.size())) >= static_cast<uint32_t>(Size))
			{
				std::array<Queue, Size> queuesArray;
				getQueues(family, queuesArray);
				return ManagedQueueArray<Size>(family, std::move(queuesArray), std::bind_front(&QueueManager::releaseQueueList, this));
			}
		}
		throw ManageException(std::source_location::current(), "Not enough queue");
	}
};

#pragma warning(pop)