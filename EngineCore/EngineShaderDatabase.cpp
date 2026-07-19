#include "pch.h"
#include <fstream>
#include <unordered_map>
#include "EngineShaderDatabase.h"
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
	auto filePath = m_shaderDirectory.string() + "/" + s_bsonShader;
	std::fstream file(filePath, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

	// read bson and compare crc : compile new shader file or changed crc
	auto bsonSize = file.tellg();
	bson_t* bsonDoc = nullptr;
	if (bsonSize == 0)
	{
		newDBFile();
	}
	else
	{
		std::vector<uint8_t> data(bsonSize);
		file.seekg(0);
		file.read(reinterpret_cast<char*>(data.data()), data.size());
		if (bsonDoc = bson_new_from_data(data.data(), data.size()))
		{
			completeDBFile(bsonDoc);
		}
	}
}

void EngineShaderDatabase::newDBFile()
{
	bson_t doc;
	bson_init(&doc);
	
	for (auto const& entry : std::filesystem::recursive_directory_iterator{ m_shaderDirectory })
	{
		if (entry.is_regular_file() && g_shaderExtensions.contains(entry.path().extension().string()))
		{
			std::fstream shaderFile(entry.path(), std::ios::in | std::ios::ate);
			if (shaderFile)
			{
				std::vector<uint8_t> data(shaderFile.tellg());
				shaderFile.seekg(0);
				shaderFile.read(reinterpret_cast<char*>(data.data()), data.size());
				const uint32_t crcValue = crc32c::Crc32c(data.data(), data.size());
				// todo
				// compile shader
				// store vkShaderModule
				// store in bson

			}
		}
	}

	auto filePath = m_shaderDirectory.string() + "/" + s_bsonShader;
	std::ofstream file(filePath, std::ios::binary);
	file.write(reinterpret_cast<const char*>(bson_get_data(&doc)), doc.len);
	bson_destroy(&doc);
}

void EngineShaderDatabase::completeDBFile(bson_t* a_bson)
{	

	//
}


std::vector<uint32_t> EngineShaderDatabase::compileShader(const std::filesystem::path& a_path)
{
	//
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
