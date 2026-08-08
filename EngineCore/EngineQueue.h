#pragma once
/***********************************************
* @headerfile EngineQueue.h
* @date 08 / 08 / 2026
* @author Roomain
************************************************/
#include <functional>
#include <vulkan/vulkan.hpp>
#include <boost/signals2.hpp>

// callbacks to release queue in vulkan
using ReleaseQueueCallback = std::function<void(int, const uint32_t)>;
using ReleasQueueListeCallback = std::function<void(int, const size_t&, const uint32_t*)>;

/*@brief release managed data signal*/
using ReleaseQueueManaged = boost::signals2::signal<void(int, const uint32_t)>;
using ReleaseQueueListManaged = boost::signals2::signal<void(int, const size_t&, const uint32_t*)>;

struct EngineQueue
{
	uint32_t queueIndex = 0;		/*!< index of queue in family*/
	VkQueue queue = VK_NULL_HANDLE;	/*!< vulkan queue*/
};