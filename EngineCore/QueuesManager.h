#pragma once
/***********************************************
* @headerfile QueuesManager.h
* @date 01 / 03 / 2026
* @author Roomain
************************************************/
#include <array>
#include <functional>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include <boost/signals2.hpp>
#include "notCopiable.h"
#include "EngineExceptions.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

struct QueueConfiguration;


using ReleaseQueueCallback = std::function<void(int, const uint32_t)>;
using ReleasQueueListeCallback = std::function<void(int, const size_t&, const uint32_t*)>;

/*@brief release managed data signal*/
using ReleaseQueueManaged = boost::signals2::signal<void(int, const uint32_t)>;
using ReleaseQueueListManaged = boost::signals2::signal<void(int, const size_t&, const uint32_t*)>;

/*@brief represents a managed queue: queue can be reused*/
class ENGINECORE_EXPORT ManagedQueue
{
	friend class QueuesManager;
protected:
	int m_queueFamily;						/*!< queue family index*/
	uint32_t m_queueIndex;					/*!< index of queue in family*/
	ReleaseQueueManaged m_releaseSignal;	/*!< release signal*/
	VkQueue m_queue = VK_NULL_HANDLE;		/*!< managed queue*/

	ManagedQueue(const int a_family, const uint32_t a_queueIndex, const VkQueue a_queue, const ReleaseQueueCallback& a_callback) :
		m_queueFamily{ a_family }, m_queueIndex{ a_queueIndex }, m_queue { a_queue }
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
	int m_queueFamily;							/*!< queue family index*/
	ReleaseQueueListManaged m_releaseSignal;	/*!< release signal*/
	std::array<VkQueue, Size> m_queues;			/*!< managed queue*/
	std::array<uint32_t, Size> m_queueIndices;	/*!< managed queue indices*/

	explicit ManagedQueueArray(const int a_family, std::array<VkQueue, Size>&& a_queues, std::array<uint32_t, Size>&& a_queueIndices, const ReleaseQueueListManaged& a_callback) :
		m_queueFamily{ a_family }, m_queues{ a_queues }, m_queueIndices{ a_queueIndices }
	{
		if(a_callback)
			m_releaseSignal.connect(a_callback);
	}

public:
	ManagedQueueArray() = delete;
	NOT_COPIABLE(ManagedQueueArray)
	explicit ManagedQueueArray(ManagedQueueArray&& other)noexcept = default;
	virtual ~ManagedQueueArray()
	{
		m_releaseSignal(m_queueFamily, Size, m_queueIndices.data());
	}
	constexpr int familyIndex()const { return m_queueFamily; }
	inline VkQueue& operator[] (const uint32_t a_index)
	{
		return m_queues[a_index];
	}
	inline VkQueue* data() { return m_queues; }
	inline const VkQueue* data()const { return m_queues; }
	inline std::array<VkQueue, Size>& get() { return m_queues; }
	inline explicit operator std::array<VkQueue, Size>& () { return m_queues; }
	inline explicit operator const std::array<VkQueue, Size>& ()const { return m_queues; }
};

/*@brief provide management of vulkan device queue*/
/*each queue in ManagedQueue/ManagedQueueArray can be reused*/
class ENGINECORE_EXPORT QueuesManager
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

public:
	// to rework ctor to create stats from device creation
	QueuesManager(const VkDevice a_dev, const VkPhysicalDevice& a_physDev, const std::vector<QueueConfiguration>& a_usedQueues);
	QueuesManager() = delete;
	NOT_COPIABLE(QueuesManager);

	[[nodiscard]] ManagedQueue createQueue(VkQueueFlags a_flags);

	template<size_t Size>
	[[nodiscard]] ManagedQueueArray<Size>&& createArray(const VkQueueFlags a_flag)
	{
		for (auto& [family, stat] : m_stats)
		{
			if ((stat.queueFlags & a_flag) == a_flag &&
				(stat.queueCount - static_cast<uint32_t>(stat.usedQueues.size())) >= static_cast<uint32_t>(Size))
			{
				//stat.inUse += static_cast<uint32_t>(Size);
				// refaire structure contenant queue et index
				std::array<VkQueue, Size> queuesArray;
				std::array<uint32_t, Size> indicesArray;
				// todo find free indices
				//vkGetDeviceQueue(m_logicalDevice, family,
				//	Size, temp.data());
				return std::move(ManagedQueueArray<Size>(family, std::move(queuesArray), std::move(indicesArray), std::bind_front(&QueuesManager::releaseQueueList, this)));
			}
		}
		throw EngineManageException(std::source_location::current(), "Not enough queue");
	}
};

#pragma warning(pop)