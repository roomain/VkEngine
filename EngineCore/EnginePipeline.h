#pragma once
/***********************************************
* @headerfile EnginePipeline.h
* @date 06 / 09 / 2026
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>

/*@brief base  class of vulkan pipeline encapsulation*/
class EnginePipeline
{
private:
	VkPipeline m_pipeline{ VK_NULL_HANDLE };
	VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
	//

public:
	constexpr VkPipeline pipeline()const { return m_pipeline; }
};