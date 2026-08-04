#include "pch.h"
#include <fstream>
#include <unordered_map>
#include "EngineShaderDatabase.h"
#include <crc32c/crc32c.h>

#pragma warning(push)
#pragma warning( disable : 4005 )
#include <slang.h>
//#include <slang-com-ptr.h>

EngineShaderDatabase::EngineShaderDatabase(const std::string& a_shaderDirectory) : 
	m_shaderDirectory(a_shaderDirectory)
{
	if (SLANG_FAILED(slang::createGlobalSession(SLANG_API_VERSION, &m_globalSession)))
	{
		EngineLog::critical("Can't create slang global session.");
		return;
	}

	m_pTarget = std::make_unique< slang::TargetDesc>();
	m_pTarget->format = SLANG_SPIRV;
	m_pTarget->profile = m_globalSession->findProfile("spirv_1_5");

	m_pSession = std::make_unique< slang::SessionDesc>();
	m_pSession->targets = m_pTarget.get();
	m_pSession->targetCount = 1;

	if (SLANG_FAILED(m_globalSession->createSession(*m_pSession.get(), &m_pSlangSession)))
	{
		EngineLog::critical("Can't create slang session.");
		return;
	}

	if (!std::filesystem::exists(m_shaderDirectory))
	{
		EngineLog::critical("Directory {} does not exists.", a_shaderDirectory);
		return;
	}
	recomputeShader();
	
	//std::ifstream file(a_shaderDirectory, std::ios::binary);
}

EngineShaderDatabase::~EngineShaderDatabase()
{
	delete m_globalSession;
	delete m_pSlangSession;
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
	// todo
	std::vector<uint32_t> outData;

	// slang shader:
	slang::IBlob* blob = nullptr;
	auto module = m_pSlangSession->loadModule(a_path.string().c_str(), &blob);
	if (!module)
	{
		if (blob)
			EngineLog::critical("{}", static_cast<const char*>(blob->getBufferPointer()));
		return outData;
	}
	else
	{
		//
	}
	return outData;
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
