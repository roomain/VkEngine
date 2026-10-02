#pragma once
/***********************************************
* @headerfile ShaderDatabase.h
* @date 29 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "enginecore_globals.h"

class Device;

#pragma warning(push)
#pragma warning( disable : 4251 )
class ENGINECORE_EXPORT ShaderDatabase
{
private:
	std::weak_ptr<Device> m_device;								/*!< device*/
	std::unordered_map<std::string, VkShaderModule> m_database;	/*!< module database*/

public:
	ShaderDatabase(const std::string& a_resourceFilename, std::shared_ptr<Device>& a_device, const std::string& a_shaderpath);
	ShaderDatabase() = delete;
	virtual ~ShaderDatabase();
	[[nodiscard]] inline VkShaderModule operator[] (const std::string& a_name) { return m_database[a_name]; }
};
#pragma warning(pop)