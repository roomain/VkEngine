#include "pch.h"
#include "ManagedQueue.h"

ManagedQueue::ManagedQueue(const int a_family, Queue&& a_queue, ReleaseQueueCallback a_callback) :
	m_queueFamily{ a_family }, m_queue{ a_queue }
{
	if(a_callback)
		m_releaseSignal.connect(a_callback);
}

ManagedQueue::~ManagedQueue()
{
	m_releaseSignal(m_queueFamily, m_queue.queueIndex);
}