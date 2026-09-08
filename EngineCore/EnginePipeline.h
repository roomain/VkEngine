#pragma once
/***********************************************
* @headerfile EnginePipeline.h
* @date 06 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vulkan/vulkan.hpp>

class EngineDevice;

/*@brief base  class of vulkan pipeline encapsulation*/
class EnginePipeline
{
protected:
	std::weak_ptr<EngineDevice> m_device;
	VkPipeline m_pipeline{ VK_NULL_HANDLE };

public:
	EnginePipeline() = default;
	virtual ~EnginePipeline()
	{
		if (m_pipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(, m_pipeline, nullptr);
	}
	constexpr VkPipeline pipeline()const { return m_pipeline; }
};