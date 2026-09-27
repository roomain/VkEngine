#include "pch.h"
#include "Device.h"
#include "Pipeline.h"

Pipeline::Pipeline(const std::string& a_name, std::weak_ptr<Device> a_device) : 
	m_pipelineName{ a_name }, m_device { a_device }
{
	// todo 
}

Pipeline::~Pipeline()
{
	if (auto device = m_device.lock(); device && m_pipeline != VK_NULL_HANDLE)
		vkDestroyPipeline(device->deviceContext().vkDevice, m_pipeline, nullptr);
}

const std::string& Pipeline::name()const
{
	return m_pipelineName;
}