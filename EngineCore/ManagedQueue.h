#pragma once
/***********************************************
* @headerfile ManagedQueue.h
* @date 08 / 08 / 2026
* @author Roomain
************************************************/
#include "Queue.h"
#include "enginecore_globals.h"
#include "notCopiable.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief represents a managed queue*/
class ENGINECORE_EXPORT ManagedQueue
{
	friend class QueueManager;
private:
	int m_queueFamily;					/*!< vulkan queue family index*/
	Queue m_queue;				/*!< managed queue*/
	ReleaseQueueManaged m_releaseSignal;/*!< release signal*/

	/*@brief queue ctor*/
	explicit ManagedQueue(const int a_family, Queue&& a_queue, ReleaseQueueCallback a_callback);

public:
	ManagedQueue() = delete;
	NOT_COPIABLE(ManagedQueue)
	explicit ManagedQueue(ManagedQueue&& other)noexcept = default;
	virtual ~ManagedQueue();
	constexpr [[nodiscard]] int familyIndex()const { return m_queueFamily; }
	inline [[nodiscard]] VkQueue& get() { return m_queue.queue; }
	inline [[nodiscard]] const VkQueue& get()const { return m_queue.queue; }
};

#pragma warning(pop)