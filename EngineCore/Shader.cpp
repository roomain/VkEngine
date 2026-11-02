#include "pch.h"
#include "Shader.h"
#include "Resources.h"
#include "enumerate.h"
#include "spirv_reflect.h"

void reflect_spirvReadVertexInput(const std::string& a_shaderName, SpvReflectShaderModule& a_reflectModule, std::vector<VkVertexInputAttributeDescription>& a_inputs)
{
	std::vector<SpvReflectInterfaceVariable*> inputReflect;
	if (SPV_REFLECT_RESULT_SUCCESS != enumerateEx(&spvReflectEnumerateInputVariables, inputReflect, SPV_REFLECT_RESULT_SUCCESS, &a_reflectModule))
	{
		spvReflectDestroyShaderModule(&a_reflectModule);
		SHADER_EXCEPT(a_shaderName.c_str(), "Can't get shader inputs");
	}
	
	for (const auto input : inputReflect)
	{
		a_inputs.emplace_back(
			VkVertexInputAttributeDescription{
			.location = input->location,
			.binding = 0,
			.format = static_cast<VkFormat>(input->format),
			.offset = 0
			}
		);
	}
}

void reflect_spirvReadBindings(const std::string& a_shaderName, SpvReflectShaderModule& a_reflectModule, const VkShaderStageFlags a_flags, std::vector<VkDescriptorSetLayoutBinding>& a_bindings)
{
	std::vector<SpvReflectDescriptorBinding*> bindingReflect;
	if (SPV_REFLECT_RESULT_SUCCESS != enumerateEx(&spvReflectEnumerateDescriptorBindings, bindingReflect, SPV_REFLECT_RESULT_SUCCESS, &a_reflectModule))
	{
		spvReflectDestroyShaderModule(&a_reflectModule);
		SHADER_EXCEPT(a_shaderName.c_str(), "Can't get shader layout bindings");
	}
	
	for (const auto binding : bindingReflect)
	{
		a_bindings.emplace_back(
			VkDescriptorSetLayoutBinding{
				.binding = binding->binding,
				.descriptorType = static_cast<VkDescriptorType>(binding->descriptor_type),
				.descriptorCount = 1,
				.stageFlags = a_flags
			}
		);
	}
}


Shader::Shader(const std::string& a_name, const VkDevice a_device, const VkShaderStageFlags a_flags, const Binary& a_shader) : m_device{ a_device },m_shaderName { a_name }
{
	SpvReflectShaderModule reflectModule;
	if (SPV_REFLECT_RESULT_SUCCESS != spvReflectCreateShaderModule(a_shader.size(), a_shader.data(), &reflectModule))
		SHADER_EXCEPT(m_shaderName.c_str(), "Can't reflect shader");

	reflect_spirvReadVertexInput(m_shaderName, reflectModule, m_inputDesc);
	reflect_spirvReadBindings(m_shaderName, reflectModule, a_flags, m_layoutBindings);
	spvReflectDestroyShaderModule(&reflectModule);

	auto moduleCreateInfo = initShaderModuleCreateInfo(a_shader);
	VK_CHECK_LOG(vkCreateShaderModule(m_device, &moduleCreateInfo, nullptr, &m_shaderModule))
}

Shader::~Shader()
{
	if (m_shaderModule != VK_NULL_HANDLE)
		vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
}