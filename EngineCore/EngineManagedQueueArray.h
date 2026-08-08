#pragma once
/***********************************************
* @headerfile EngineManagedQueueArray.h
* @date 08 / 08 / 2026
* @author Roomain
************************************************/
#include <array>
#include "EngineQueue.h"
#include "notCopiable.h"

template<size_t Size>
class EngineManagedQueueArray
{
	friend class EngineQueueManager;
private:
	int m_queueFamily;							/*!< vulkan queue family index*/
	std::array<EngineQueue, Size> m_queueArray;	/*!< managed queue*/
	ReleaseQueueListManaged m_releaseSignal;	/*!< release signal*/

	explicit EngineManagedQueueArray(const int a_familyIndex, std::array<EngineQueue, Size>&& a_queues, const ReleaseQueueListManaged& a_releaseCallback) :
		m_queueFamily{ a_familyIndex }, m_queueArray{ a_queues }
	{
		if (a_releaseCallback)
			m_releaseSignal.connect(a_releaseCallback);
	}

public:
	EngineManagedQueueArray() = delete;
	NOT_COPIABLE(EngineManagedQueueArray)
	explicit EngineManagedQueueArray(EngineManagedQueueArray&& other)noexcept = default;
	virtual ~EngineManagedQueueArray()
	{
		int index = 0;
		std::array<uint32_t, Size> queueIndicies;
		for (const auto [queue, queueIndex] : m_queueArray)
		{
			queueIndicies[index] = queueIndex;
			++index;
		}
		m_releaseSignal(m_queueFamily, Size, queueIndicies.data());
	}
	constexpr [[nodiscard]] int familyIndex()const { return m_queueFamily; }
	constexpr [[nodiscard]] VkQueue operator [] (const uint32_t a_index) { return m_queueArray[a_index].queue; }
	constexpr [[nodiscard]] const VkQueue operator [] (const uint32_t a_index)const { return m_queueArray[a_index].queue; }

	using const_iterator = std::array<EngineQueue, Size>::const_iterator;
	[[nodiscard]] const_iterator cbegin() const { return m_queueArray.cbegin(); }
	[[nodiscard]] const_iterator cend() const { return m_queueArray.cend(); }
};