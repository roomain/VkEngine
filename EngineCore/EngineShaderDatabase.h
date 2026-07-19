#pragma once
/***********************************************
* @headerfile EngineShaderDatabase.h
* @date 14 / 07 / 2026
* @author Roomain
************************************************/
#include <filesystem>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <set>
#include <vulkan/vulkan.hpp>
#include <libbson-1.0/bson.h>
#include "notCopiable.h"
#include "macros/iterators.h"

using ShaderDatabase = std::unordered_map<std::string, VkShaderModule>;
using ShaderLoaderDatabase = std::unordered_map<std::string, std::function < std::vector<uint32_t>(const std::filesystem::path&)>>;

class EngineShaderDatabase
{
private:
	static inline const std::set<std::string> g_shaderExtensions = {
		".glsl", ".vert", ".frag", ".geom", ".tesc", ".tese", ".comp",
		".mesh", ".task",
		".rgen", ".rchit", ".rahit", ".rmiss", ".rint", ".rcall",
		".hlsl", ".fx", ".fxh",
		".cg", ".cginc",
		".metal", ".msl",
		".slang", ".slangh",
		".wgsl",
		".shader", ".compute",
		".vs", ".ps", ".gs", ".hs", ".ds", ".cs",
		".fs", ".vsh", ".psh", ".fsh",
		".glslinc", ".shaderinc"
	};

	std::filesystem::path m_shaderDirectory;/*!< path to shader directory*/
	std::filesystem::path m_bsonFile;		/*!< path to bson*/
	ShaderDatabase m_shaderDatabase;		/*!< shader modules by filename*/
	
	static inline const std::string s_bsonShader = "shader_database.bson"; /*!< database*/

	/*@brief recompute new or updated shaders in directory*/
	void recomputeShader();
	void newDBFile();
	void completeDBFile(bson_t* a_bson);

	static std::vector<uint32_t> compileShader(const std::filesystem::path& a_path);
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