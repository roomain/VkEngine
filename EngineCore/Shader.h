#pragma once
/***********************************************
* @headerfile Shader.h
* @date 02 / 11 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <string>
#include <vulkan/vulkan.hpp>
#include "notCopiable.h"
#include "enginecore_globals.h"

using Binary = std::vector<char>;

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief shader data for pipeline construction*/
class ENGINECORE_EXPORT Shader
{
private:
	VkDevice m_device;											/*!< vulkan device*/
	std::string m_shaderName;									/*!< shader name*/
	VkShaderStageFlagBits m_shaderType;							/*!< shader type*/
	VkShaderModule m_shaderModule;								/*!< vulkan shader module*/

	std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;	/*!< shader bindings*/
	std::vector<VkVertexInputAttributeDescription> m_inputDesc;	/*!< shader input*/

public:
	Shader() = delete;
	explicit Shader(const std::string& a_name, const VkDevice a_device, const VkShaderStageFlags a_flags, const Binary& a_shader);
	~Shader();
	NOT_COPIABLE(Shader)
	constexpr VkShaderStageFlagBits type()const noexcept { return m_shaderType; }
	constexpr VkShaderModule shaderModule()const noexcept { return m_shaderModule; }
	inline const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings()const noexcept { return m_layoutBindings; }
	inline const std::vector<VkVertexInputAttributeDescription>& inputDesc()const noexcept { return m_inputDesc; }
};
#pragma warning(pop)