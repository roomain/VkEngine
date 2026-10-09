#pragma once
/***********************************************
* @headerfile ShaderDatabase.h
* @date 29 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "ShaderResourceCompiler.h"
#include "enginecore_globals.h"

class Device;

#pragma warning(push)
#pragma warning( disable : 4251 )
class ENGINECORE_EXPORT ShaderDatabase
{
private:
	std::string m_shadersDirectory;								/*<! shader directory*/
	std::weak_ptr<Device> m_device;								/*!< device*/
	std::unordered_map<std::string, VkShaderModule> m_database;	/*!< module database*/
	std::unique_ptr<ShaderResourceCompiler> m_compiler;

public:
	ShaderDatabase(const std::string& a_resourceFilename, std::shared_ptr<Device>& a_device, const std::string& a_shaderpath);
	ShaderDatabase() = delete;
	virtual ~ShaderDatabase();
	[[nodiscard]] inline VkShaderModule operator[] (const std::string& a_name) { return m_database[a_name]; }
	bool addShaders(const std::vector<std::string>& a_files);
	bool addShader(const std::string& a_file);
};
#pragma warning(pop)