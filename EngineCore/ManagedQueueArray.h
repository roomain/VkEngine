#pragma once
/***********************************************
* @headerfile ManagedQueueArray.h
* @date 08 / 08 / 2026
* @author Roomain
************************************************/
#include <array>
#include "Queue.h"
#include "notCopiable.h"

template<size_t Size>
class ManagedQueueArray
{
	friend class QueueManager;
private:
	int m_queueFamily;							/*!< vulkan queue family index*/
	std::array<Queue, Size> m_queueArray;	/*!< managed queue*/
	ReleaseQueueListManaged m_releaseSignal;	/*!< release signal*/

	explicit ManagedQueueArray(const int a_familyIndex, std::array<Queue, Size>&& a_queues, ReleasQueueListCallback a_releaseCallback) :
		m_queueFamily{ a_familyIndex }, m_queueArray{ a_queues }
	{
		if (a_releaseCallback)
			m_releaseSignal.connect(a_releaseCallback);
	}

public:
	ManagedQueueArray() = delete;
	NOT_COPIABLE(ManagedQueueArray);
	explicit ManagedQueueArray(ManagedQueueArray&& a_other)noexcept : m_queueFamily{ a_other.m_queueFamily },
		m_queueArray{ std::move(a_other.m_queueArray) }, m_releaseSignal{ std::move(a_other.m_releaseSignal) }
	{
		a_other.m_queueFamily = -1;
	}

	virtual ~ManagedQueueArray()
	{
		if (m_queueFamily < 0)
			return;

		int index = 0;
		std::array<uint32_t, Size> queueIndicies;
		for (const auto [queueIndex, queue] : m_queueArray)
		{
			queueIndicies[index] = queueIndex;
			++index;
		}
		m_releaseSignal(m_queueFamily, Size, queueIndicies.data());
	}
	constexpr [[nodiscard]] int familyIndex()const { return m_queueFamily; }
	constexpr [[nodiscard]] VkQueue operator [] (const uint32_t a_index) { return m_queueArray[a_index].queue; }
	constexpr [[nodiscard]] const VkQueue operator [] (const uint32_t a_index)const { return m_queueArray[a_index].queue; }
	constexpr [[nodiscard]] size_t size()const noexcept { return Size; }

	using const_iterator = std::array<Queue, Size>::const_iterator;
	[[nodiscard]] const_iterator cbegin() const { return m_queueArray.cbegin(); }
	[[nodiscard]] const_iterator cend() const { return m_queueArray.cend(); }
};