#include "pch.h"
#include "Shader.h"
#include "Pipeline.h"

Pipeline::Pipeline(const std::string& a_name, const std::vector<ShaderPtr>& a_shaders) :
	m_pipelineName{ a_name }
{
	if (!a_shaders.empty())
	{
		//VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		m_device = a_shaders[0]->device();
		for (const auto& pShader : a_shaders)
		{
			//
		}
	}
}

Pipeline::~Pipeline()
{
	if (m_pipeline != VK_NULL_HANDLE)
		vkDestroyPipeline(m_device, m_pipeline, nullptr);
}

const std::string& Pipeline::name()const
{
	return m_pipelineName;
}