#pragma once
/***********************************************
* @headerfile Pipeline.h
* @date 06 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>
#include "enginecore_globals.h"

class Device;
class ShaderDatabase;


#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief base  class of vulkan pipeline encapsulation*/
class ENGINECORE_EXPORT Pipeline
{
protected:
	std::string m_pipelineName;
	std::weak_ptr<Device> m_device;
	VkPipeline m_pipeline{ VK_NULL_HANDLE };

public:
	Pipeline() = delete;
	explicit Pipeline(const std::string& a_name, std::weak_ptr<Device> a_device);
	virtual ~Pipeline();
	virtual void setup(ShaderDatabase& a_database) = 0;
	constexpr VkPipeline pipeline()const { return m_pipeline; }
	[[nodiscard]] const std::string& name()const;
};




#pragma warning(pop)