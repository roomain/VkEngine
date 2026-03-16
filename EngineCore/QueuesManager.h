#pragma once
/***********************************************
* @headerfile QueuesManager.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <array>
#include <functional>
#include <vulkan/vulkan.hpp>
#include <boost/signals2.hpp>
#include "notCopiable.h"
#include "EngineExceptions.h"

/*@brief release managed data signal*/
using ReleaseManaged = boost::signals2::signal<void(int, const size_t&)>;
using ReleaseCallback = std::function<void(int, const size_t&)>;

/*@brief represents a managed queue: queue can be reused*/
class ManagedQueue
{
	friend class QueuesManager;
protected:
	int m_queueFamily;					/*!< queue family index*/
	ReleaseManaged m_releaseSignal;		/*!< release signal*/
	VkQueue m_queue = VK_NULL_HANDLE;	/*!< managed queue*/

	ManagedQueue(int a_family, VkQueue a_queue, ReleaseCallback a_callback) : m_queueFamily{ a_family }, m_queue{ a_queue }
	{
		m_releaseSignal.connect(a_callback);
	}

public:
	ManagedQueue() = delete;
	NOT_COPIABLE(ManagedQueue)
	virtual ~ManagedQueue()
	{
		m_releaseSignal(m_queueFamily, 1);
	}
	constexpr int familyIndex()const { return m_queueFamily; }
	inline VkQueue& get() { return m_queue; }
	inline operator VkQueue& () { return m_queue; }
	inline operator const VkQueue& ()const { return m_queue; }
};

template<size_t Size>
class ManagedQueueArray
{
	friend class QueuesManager;
protected:
	int m_queueFamily;					/*!< queue family index*/
	ReleaseManaged m_releaseSignal;		/*!< release signal*/
	std::array<VkQueue, Size> m_queues;	/*!< managed queue*/

	explicit ManagedQueueArray(int a_family, std::array<VkQueue, Size>&& a_queues, ReleaseCallback a_callback) :
		m_queueFamily{ a_family }, m_queues{ a_queues }
	{
		m_releaseSignal.connect(a_callback);
	}

public:
	ManagedQueueArray() = delete;
	NOT_COPIABLE(ManagedQueueArray)
	virtual ~ManagedQueueArray()
	{
		m_releaseSignal(m_queueFamily, Size);
	}
	constexpr int familyIndex()const { return m_queueFamily; }
	inline VkQueue& operator[] (const uint32_t a_index)
	{
		return m_queues[a_index];
	}
	inline std::array<VkQueue, Size>& get() { return m_queues; }
	inline operator std::array<VkQueue, Size>& () { return m_queues; }
	inline operator const std::array<VkQueue, Size>& ()const { return m_queues; }
};

/*@brief provide management of vulkan device queue*/
class QueuesManager
{
private:
	struct QueueFamilyStatistics : VkQueueFamilyProperties
	{
		uint32_t inUse = 0;           /*!< number of ManagedType in use*/
	};

	VkDevice m_logicalDevice;								/*!< logical device*/
	std::unordered_map<int, QueueFamilyStatistics> m_stats; /*!< statistics per family*/

	void release(const int a_familyIndex, const size_t& a_count);

public:
	QueuesManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev);
	QueuesManager() = delete;
	NOT_COPIABLE(QueuesManager);

	ManagedQueue createQueue(VkQueueFlags a_flags);

	template<uint32_t Size>
	ManagedQueueArray<Size> createArray(VkQueueFlags a_flag)
	{
		for (auto& [family, stat] : m_stats)
		{
			if ((stat.queueFlags & a_flag) == a_flag &&
				(stat.queueCount - stat.inUse) >= static_cast<uint32_t>(Size))
			{
				stat.inUse += static_cast<uint32_t>(Size);
				std::array<VkQueue, Size> temp;
				vkGetDeviceQueue(m_logicalDevice, family,
					Size, temp.data());
				return array(family, temp, std::bind_front(&QueuesManager::release, this));
			}
		}
		throw EngineManageException(std::source_location::current(), "Not enough queue");
	}
};