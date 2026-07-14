#pragma once
/***********************************************
* @headerfile EngineShaderDatabase.h
* @date 14 / 07 / 2026
* @author Roomain
************************************************/
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "notCopiable.h"
#include "macros/iterators.h"

using ShaderDatabase = std::unordered_map<std::string, VkShaderModule>;

class EngineShaderDatabase
{
private:
	std::filesystem::path m_shaderDirectory;/*!< path to shader directory*/
	std::filesystem::path m_bsonFile;		/*!< path to bson*/
	ShaderDatabase m_shaderDatabase;		/*!< shader modules by filename*/
	
	static inline const std::string s_bsonShader = "shader_database.bson"; /*!< database*/

	/*@brief recompute new or updated shaders in directory*/
	void recomputeShader();

	static std::vector<uint32_t> loadSpirv(const std::filesystem::path& a_path);
	static std::vector<uint32_t> loadSlang(const std::filesystem::path& a_path);
	static std::vector<uint32_t> loadGLSL(const std::filesystem::path& a_path);

public:
	EngineShaderDatabase() = delete;
	EngineShaderDatabase(const std::string& a_shaderDirectory);
	NOT_COPIABLE(EngineShaderDatabase)
	DEFINE_ITER(ShaderDatabase, m_shaderDatabase)
	DEFINE_CONST_ITER(ShaderDatabase, m_shaderDatabase)
	VkShaderModule shaderModule(const std::string& a_shaderName);
};