#include "pch.h"
#include <fstream>
#include "EngineShaderDatabase.h"

EngineShaderDatabase::EngineShaderDatabase(const std::string& a_shaderDirectory) : 
	m_shaderDirectory(a_shaderDirectory)
{
	if (!std::filesystem::exists(m_shaderDirectory))
	{
		EngineLog::critical("Directory {} does not exists.", a_shaderDirectory);
		return;
	}

	auto filePath = m_shaderDirectory.string()  + "/" + s_bsonShader;
	std::fstream file(filePath, std::ios::binary | std::ios::in | std::ios::out);
	if (!file)
	{
		EngineLog::critical("Can't openfile {}", filePath);
		return;
	}
	//std::ifstream file(a_shaderDirectory, std::ios::binary);
}

void EngineShaderDatabase::recomputeShader()
{
	// todo
}

std::vector<uint32_t> EngineShaderDatabase::loadSpirv(const std::filesystem::path& a_path)
{
	// todo
	std::vector<uint32_t> outData;
	// todo
	return outData;
}

std::vector<uint32_t> EngineShaderDatabase::loadSlang(const std::filesystem::path& a_path)
{
	// todo
	std::vector<uint32_t> outData;
	// todo
	return outData;
}

std::vector<uint32_t> EngineShaderDatabase::loadGLSL(const std::filesystem::path& a_path)
{
	// todo
	std::vector<uint32_t> outData;
	// todo
	return outData;
}
