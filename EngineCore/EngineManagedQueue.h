#pragma once
/***********************************************
* @headerfile EngineManagedQueue.h
* @date 08 / 08 / 2026
* @author Roomain
************************************************/
#include "EngineQueue.h"
#include "enginecore_globals.h"
#include "notCopiable.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief represents a managed queue*/
class ENGINECORE_EXPORT EngineManagedQueue
{
	friend class EngineQueueManager;
private:
	int m_queueFamily;					/*!< vulkan queue family index*/
	EngineQueue m_queue;				/*!< managed queue*/
	ReleaseQueueManaged m_releaseSignal;/*!< release signal*/

	/*@brief queue ctor*/
	explicit EngineManagedQueue(const int a_family, EngineQueue&& a_queue, const ReleaseQueueCallback& a_callback);

public:
	EngineManagedQueue() = delete;
	NOT_COPIABLE(EngineManagedQueue)
	explicit EngineManagedQueue(EngineManagedQueue&& other)noexcept = default;
	virtual ~EngineManagedQueue();
	constexpr [[nodiscard]] int familyIndex()const { return m_queueFamily; }
	inline [[nodiscard]] VkQueue& get() { return m_queue.queue; }
	inline [[nodiscard]] const VkQueue& get()const { return m_queue.queue; }
};

#pragma warning(pop)