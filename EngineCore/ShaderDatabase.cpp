#include "pch.h"
#include <filesystem>
#include <algorithm>
#include "ShaderDatabase.h"
#include "ResourceChecker.h"
#include "ResourceFile.h"
#include "Device.h"

std::vector<std::string> g_shaderExt = {
				".slang",
				".vert",
				".tesc",
				".tese",
				".geom",
				".frag",
				".comp",
				".task",
				".mesh"
		};

ShaderDatabase::ShaderDatabase(const std::string& a_resourceFilename, std::shared_ptr<Device>& a_device, const std::string& a_shaderpath) : m_device{ a_device },
	m_shadersDirectory{ a_shaderpath }
{
	m_compiler = std::make_unique<ShaderResourceCompiler>();
	std::replace(m_shadersDirectory.begin(), m_shadersDirectory.end(), '/', '\\');
	if (a_device)
	{
		CheckerParameters checkerParam{
			.callbacks {
				.logCallback = [](const std::string& a_log) { Log::error("{}", a_log); },
				.rangeCallback = nullptr,
				.errorCounterCallback = [](const unsigned int a_count) { Log::error("{} errors", a_count); }
			},
			.extensions = g_shaderExt
		};
		ResourceFile file;
		file.loadAllFile(a_resourceFilename);
		ResourceChecker checker(checkerParam, a_shaderpath);
		for (auto iter = file.binaryBegin(); iter != file.binaryEnd(); iter++)
		{
			auto moduleCreateInfo = initShaderModuleCreateInfo(iter->second);
			VkShaderModule shaderModule = VK_NULL_HANDLE;
			VK_CHECK_LOG(vkCreateShaderModule(a_device->deviceContext().vkDevice, &moduleCreateInfo, nullptr, &shaderModule))
			if (shaderModule != VK_NULL_HANDLE)
				m_database.emplace(iter->first, shaderModule);
		}

		if (file.modified())
			file.save();
	}
}

ShaderDatabase::~ShaderDatabase()
{
	if (auto pDevice = m_device.lock())
	{
		for (auto& [name, shaderModule] : m_database)
			vkDestroyShaderModule(pDevice->deviceContext().vkDevice, shaderModule, nullptr);
	}
}

bool ShaderDatabase::addShaders(const std::vector<std::string>& a_files)
{
	bool bOk = true;
	for (const auto& filePath : a_files)
	{
		if (!(bOk &= addShader(filePath)))
			break;
	}
	return bOk;
}

bool ShaderDatabase::addShader(const std::string& a_file)
{
	if (auto pDevice = m_device.lock())
	{
		auto expectedBin = m_compiler->compile(a_file);
		if (expectedBin.has_value())
		{
			auto moduleCreateInfo = initShaderModuleCreateInfo(a_file);
			VkShaderModule shaderModule = VK_NULL_HANDLE;
			VK_CHECK_LOG(vkCreateShaderModule(pDevice->deviceContext().vkDevice, &moduleCreateInfo, nullptr, &shaderModule))
			if (shaderModule != VK_NULL_HANDLE && m_database.try_emplace(std::filesystem::path(a_file).filename(), shaderModule).second)
			{
				// 2 copy if not present in shader path
				// to test no overwrite
				return std::filesystem::copy_file(std::filesystem::path(a_file), std::filesystem::path(m_shadersDirectory));
			}
		}
	}
	return false;
}