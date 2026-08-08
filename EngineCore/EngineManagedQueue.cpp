#include "pch.h"
#include "EngineManagedQueue.h"

EngineManagedQueue::EngineManagedQueue(const int a_family, EngineQueue&& a_queue, const ReleaseQueueCallback& a_callback) :
	m_queueFamily{ a_family }, m_queue{ a_queue }
{
	if(a_callback)
		m_releaseSignal.connect(a_callback);
}

EngineManagedQueue::~EngineManagedQueue()
{
	m_releaseSignal(m_queueFamily, m_queue.queueIndex);
}