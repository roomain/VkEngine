#include "pch.h"
#include "EngineDevice.h"
#include "EnginePipeline.h"

EnginePipeline::EnginePipeline(const std::string& a_name, std::weak_ptr<EngineDevice> a_device) : 
	m_pipelineName{ a_name }, m_device { a_device }
{
	// todo 
}

EnginePipeline::~EnginePipeline()
{
	if (auto device = m_device.lock(); device && m_pipeline != VK_NULL_HANDLE)
		vkDestroyPipeline(device->deviceContext().vkDevice, m_pipeline, nullptr);
}

const std::string& EnginePipeline::name()const
{
	return m_pipelineName;
}