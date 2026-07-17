#include "pch.h"
#include <fstream>
#include <set>
#include "EngineShaderDatabase.h"
#include <libbson-1.0/bson.h>
#include <crc32c/crc32c.h>



EngineShaderDatabase::EngineShaderDatabase(const std::string& a_shaderDirectory) : 
	m_shaderDirectory(a_shaderDirectory)
{
	if (!std::filesystem::exists(m_shaderDirectory))
	{
		EngineLog::critical("Directory {} does not exists.", a_shaderDirectory);
		return;
	}
	recomputeShader();
	
	//std::ifstream file(a_shaderDirectory, std::ios::binary);
}

void EngineShaderDatabase::recomputeShader()
{
	static const std::set<std::string> shaderExtensions =
	{
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

	auto filePath = m_shaderDirectory.string() + "/" + s_bsonShader;
	std::fstream file(filePath, std::ios::binary | std::ios::in | std::ios::out);
	if (!file)
	{
		EngineLog::critical("Can't open file {}", filePath);
		return;
	}

	std::unordered_map<std::string, uint32_t> crcByFile;

	// list shader files and get CRC
	for (auto const& entry : std::filesystem::recursive_directory_iterator{ m_shaderDirectory })
	{
		if (entry.is_regular_file() && shaderExtensions.contains(entry.path().extension().string()))
		{
			std::fstream shaderFile(entry.path(), std::ios::in | std::ios::ate);
			if (shaderFile)
			{
				std::vector<uint8_t> data(shaderFile.tellg());
				shaderFile.seekg(0);
				shaderFile.read(reinterpret_cast<char*>(data.data()), data.size());
				crcByFile.emplace(entry.path(), crc32c::Crc32c(data.data(), data.size()));
			}
		}
	}

	// read bson and compare crc : compile new shader file or changed crc
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
