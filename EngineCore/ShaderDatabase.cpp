#include "pch.h"
#include <filesystem>
#include <algorithm>
#include "ShaderDatabase.h"
#include "ResourceChecker.h"
#include "Shader.h"
#include "ShaderResourceCompiler.h"
#include "ResourceFile.h"
#include "Device.h"
#include "enumerate.h"

const std::vector<std::string> g_shaderExt = {
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


VkShaderStageFlags shaderTypeFromExtension(const std::string_view a_shaderName)
{
	VkShaderStageFlags flag = 0;
	if (a_shaderName.contains(".vert"))
		flag |= VK_SHADER_STAGE_VERTEX_BIT;
	if (a_shaderName.contains(".tesc"))
		flag |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (a_shaderName.contains(".tese"))
		flag |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (a_shaderName.contains(".geom"))
		flag |= VK_SHADER_STAGE_GEOMETRY_BIT;
	if (a_shaderName.contains(".frag"))
		flag |= VK_SHADER_STAGE_FRAGMENT_BIT;
	if (a_shaderName.contains(".comp"))
		flag |= VK_SHADER_STAGE_COMPUTE_BIT;
	if (a_shaderName.contains(".task"))
		flag |= VK_SHADER_STAGE_TASK_BIT_EXT;
	if (a_shaderName.contains(".mesh"))
		flag |= VK_SHADER_STAGE_MESH_BIT_EXT;

	return flag;
}


ShaderDatabase::ShaderDatabase(const std::string& a_resourceFilename, std::shared_ptr<Device>& a_device, const std::string& a_shaderpath) : m_device{ a_device },
	m_shadersDirectory{ a_shaderpath }
{
	m_compiler = std::make_unique<ShaderResourceCompiler>();
	std::replace(m_shadersDirectory.begin(), m_shadersDirectory.end(), '/', '\\');
	if (a_device)
	{
		const CheckerParameters checkerParam{
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
			m_database.emplace(iter->first, std::make_shared<Shader>(iter->first, a_device->deviceContext().vkDevice, shaderTypeFromExtension(iter->first), iter->second));

		if (file.modified())
			file.save();
	}
}

ShaderDatabase::~ShaderDatabase()
{
	//
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
			auto file = std::filesystem::path(a_file).filename().string();
			m_database.emplace(file, std::make_shared<Shader>(file, pDevice->deviceContext().vkDevice, shaderTypeFromExtension(file), expectedBin.value()));
			// to test no overwrite
			return std::filesystem::copy_file(std::filesystem::path(a_file), std::filesystem::path(m_shadersDirectory));
		}
	}
	return false;
}