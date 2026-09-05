#include "GLSLResourceCompiler.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <format>
#include <fstream>
#include <filesystem>

int GLSLResourceCompiler::getShaderTypeFromFile(const std::string& a_filename)
{
	std::filesystem::path path(a_filename);
	const auto extension = path.extension().string();

	if (extension.compare(".vert") == 0)
		return EShLangVertex;
	else if (extension.compare(".tesc") == 0)
		return EShLangTessControl;
	else if (extension.compare(".tese") == 0)
		return EShLangTessEvaluation;
	else if (extension.compare(".geom") == 0)
		return EShLangGeometry;
	else if (extension.compare(".frag") == 0)
		return EShLangFragment;
	else if (extension.compare(".comp") == 0)
		return EShLangCompute;
	else if (extension.compare(".task") == 0)
		return EShLangTask;
	else if (extension.compare(".mesh") == 0)
		return EShLangMesh;

	return -1;
}

bool GLSLResourceCompiler::isFileAvailable(const std::string& a_filename)
{
	return GLSLResourceCompiler::getShaderTypeFromFile(a_filename) != -1;
}

GLSLResourceCompiler::GLSLResourceCompiler() : Compiler{ }
{
	m_bIsInittialized = glslang::InitializeProcess();
}

GLSLResourceCompiler::~GLSLResourceCompiler()
{
	if(m_bIsInittialized)
		glslang::FinalizeProcess();
}

std::expected<Binary, CompilerError> GLSLResourceCompiler::compile(const std::string& a_filename)
{
	if (!m_bIsInittialized)
		return std::unexpected(CompilerError{ CompilerError::Type::InitError,  "GLSLANG not initialized" });

	std::ifstream file(a_filename, std::ios::ate);
	if (!file.good())
		return std::unexpected(CompilerError{ CompilerError::Type::InitError,  "Can't read file" });
	auto fileSize = file.tellg();
	file.seekg(0);

	std::vector<char> code(fileSize);
	file.read(code.data(), fileSize);

	glslang::TShader shader(EShLangVertex);
	const char* src = code.data();
	shader.setStrings(&src, 1);

	// Options GLSL → SPIR-V
	const auto msgTypes = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
	
	if (!shader.parse(GetDefaultResources(),
		450,              // version GLSL
		false,            // forward compatible
		msgTypes))
	{
		return std::unexpected(CompilerError{ CompilerError::Type::CompileError, shader.getInfoLog() });
	}

	glslang::TProgram program;
	program.addShader(&shader);

	if (!program.link(msgTypes))
		return std::unexpected(CompilerError{ CompilerError::Type::LinkError, shader.getInfoLog() });

	// Génération du SPIR-V
	std::vector<uint32_t> spirv;

	glslang::SpvOptions spvOptions{
		.generateDebugInfo = true,
		.disableOptimizer = false,
		.optimizeSize = true,
		.optimizePerformance = true
	};
	
	spv::SpvBuildLogger logger;
	const auto glslType = static_cast<EShLanguage>(getShaderTypeFromFile(a_filename));
	glslang::GlslangToSpv(
		*program.getIntermediate(glslType),
		spirv,
		&logger,
		&spvOptions
	);

	if (spirv.empty())
		return std::unexpected(CompilerError{ CompilerError::Type::SpirvGen, logger.getAllMessages() });

	Binary spirvBin(spirv.size() * sizeof(uint32_t));
	memcpy_s(spirvBin.data(), spirvBin.size(), spirv.data(), spirvBin.size());
	return spirvBin;
}