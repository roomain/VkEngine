#pragma once
/***********************************************
* @headerfile EnginePipeline.h
* @date 06 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>
#include "enginecore_globals.h"

class EngineDevice;
class Shader;

struct ShaderParameters
{
	VkShaderModule shader;
	VkShaderStageFlagBits shaderFlag;
};

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief base  class of vulkan pipeline encapsulation*/
class ENGINECORE_EXPORT EnginePipeline
{
private:
	std::string m_pipelineName;
	std::weak_ptr<EngineDevice> m_device;
	VkPipeline m_pipeline{ VK_NULL_HANDLE };

public:
	EnginePipeline() = delete;
	explicit EnginePipeline(const std::string& a_name, std::weak_ptr<EngineDevice> a_device);
	virtual ~EnginePipeline();
	constexpr VkPipeline pipeline()const { return m_pipeline; }
	[[nodiscard]] const std::string& name()const;
};



//class ENGINECORE_EXPORT EngineMeshPipeline : public EnginePipeline
//{
//private:
//	std::string m_pipelineName;
//	std::weak_ptr<EngineDevice> m_device;
//	VkPipeline m_pipeline{ VK_NULL_HANDLE };
//
//public:
//	EngineMeshPipeline() = delete;
//	explicit EngineMeshPipeline(const std::string& a_name, std::weak_ptr<EngineDevice> a_device);
//	virtual ~EngineMeshPipeline();
//	constexpr VkPipeline pipeline()const { return m_pipeline; }
//	[[nodiscard]] const std::string& name()const;
//};
#pragma warning(pop)