#include "SlangResourceCompiler.h"
#include <format>
#include <filesystem>


bool SlangResourceCompiler::isFileAvailable(const std::string& a_filename)
{
	std::filesystem::path path(a_filename);
	return path.extension().string().compare(".slang") == 0;
}

SlangResourceCompiler::SlangResourceCompiler() : Compiler{}
{
	// global session
	if (SLANG_SUCCEEDED(slang::createGlobalSession(m_globalSession.writeRef())))
	{
		slang::TargetDesc target;
		slang::SessionDesc sessionDesc;
		target.format = SLANG_SPIRV;
		target.profile = m_globalSession->findProfile("spirv_1_5");
		sessionDesc.targetCount = 1;
		sessionDesc.targets = &target;
		m_globalSession->createSession(sessionDesc, m_session.writeRef());
	}

}


SlangResourceCompiler::~SlangResourceCompiler()
{
	slang::shutdown();
}

std::expected<Binary, CompilerError> SlangResourceCompiler::compile(const std::string& a_filename)
{
	if (!m_session)
		return std::unexpected(CompilerError{ CompilerError::Type::InitError,  "No slang session" });

	Slang::ComPtr<slang::IBlob> diagnostics;
	//---------------------------------------
	// Load module
	//---------------------------------------

	auto slangModule = m_session->loadModule(a_filename.c_str(), diagnostics.writeRef());

	if (!slangModule)
	{
		if (diagnostics)
			return std::unexpected(CompilerError{ CompilerError::Type::CompileError,  
				static_cast<const char*>(diagnostics->getBufferPointer()) });


		return std::unexpected(CompilerError{ CompilerError::Type::CompileError,
				"No slang module" });
	}

	//---------------------------------------
	// Entry point
	//---------------------------------------

	const SlangInt32 entryPointCount = slangModule->getDefinedEntryPointCount();
	std::vector<Slang::ComPtr<slang::IEntryPoint>> entryPointsList(entryPointCount);
	for (SlangInt32 index = 0; index < entryPointCount; ++index)
	{
		if (SLANG_FAILED(slangModule->getDefinedEntryPoint(index, entryPointsList[index].writeRef())))
			return std::unexpected(CompilerError{ CompilerError::Type::CompileError,
				"Can't get entry point"});
	}

	if (entryPointsList.empty())
		return std::unexpected(CompilerError{ CompilerError::Type::CompileError,
			"Entry point not found" });

	//---------------------------------------
	// Compose
	//---------------------------------------
	std::array<slang::IComponentType* const, 2> components = { slangModule, entryPointsList[0] };
	Slang::ComPtr<slang::IComponentType> program;

	if (SLANG_FAILED(m_session->createCompositeComponentType(components.data(), 2, program.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			return std::unexpected(CompilerError{ CompilerError::Type::CompileError,
				static_cast<const char*>(diagnostics->getBufferPointer()) });

		return std::unexpected(CompilerError{ CompilerError::Type::CompileError,
				"Create Composite Component"});
	}

	//---------------------------------------
	// Link
	//---------------------------------------
	Slang::ComPtr<slang::IComponentType> linkedProgram;
	if (SLANG_FAILED(program->link(linkedProgram.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			return std::unexpected(CompilerError{ CompilerError::Type::LinkError,
				static_cast<const char*>(diagnostics->getBufferPointer()) });

		return std::unexpected(CompilerError{ CompilerError::Type::LinkError,
			"Unknown"});
	}

	//---------------------------------------
	// Generate SPIR-V
	//---------------------------------------
	Slang::ComPtr<slang::IBlob> spirv;
	if (SLANG_FAILED(linkedProgram->getEntryPointCode(0, 0, spirv.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			return std::unexpected(CompilerError{ CompilerError::Type::SpirvGen,
				static_cast<const char*>(diagnostics->getBufferPointer()) });


		return std::unexpected(CompilerError{ CompilerError::Type::SpirvGen,
			"Unknown" });
	}

	Binary bin(spirv->getBufferSize());
	memcpy(bin.data(), spirv->getBufferPointer(), spirv->getBufferSize());
	return bin;
}