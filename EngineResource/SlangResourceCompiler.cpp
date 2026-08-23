#include "SlangResourceCompiler.h"
#include <format>

SlangResourceCompiler::SlangResourceCompiler(LogCallback a_log, LogCallback a_errorLog) : m_log{a_log},
m_logError{a_errorLog}
{
	// global session
	if (SLANG_FAILED(slang::createGlobalSession(m_globalSession.writeRef())))
	{
		m_logError("Can't create slang global session.\n");
		return;
	}

	slang::TargetDesc target;
	slang::SessionDesc sessionDesc;
	target.format = SLANG_SPIRV;
	target.profile = m_globalSession->findProfile("spirv_1_5");
	sessionDesc.targetCount = 1;
	sessionDesc.targets = &target;
	m_globalSession->createSession(sessionDesc, m_session.writeRef());
}


SlangResourceCompiler::~SlangResourceCompiler()
{
	slang::shutdown();
}

Binary SlangResourceCompiler::compile(const std::string& a_filename)
{
	if (!m_session)
	{
		m_logError("No slan session.\n");
		return Binary{};
	}

	Slang::ComPtr<slang::IBlob> diagnostics;
	//---------------------------------------
	// Load module
	//---------------------------------------

	auto slangModule = m_session->loadModule(a_filename.c_str(), diagnostics.writeRef());

	if (!slangModule)
	{
		if (diagnostics)
			m_logError(static_cast<const char*>(diagnostics->getBufferPointer()));

		return Binary{};
	}

	//---------------------------------------
	// Entry point
	//---------------------------------------

	const SlangInt32 entryPointCount = slangModule->getDefinedEntryPointCount();
	std::vector<Slang::ComPtr<slang::IEntryPoint>> entryPointsList(entryPointCount);
	for (SlangInt32 index = 0; index < entryPointCount; ++index)
	{
		if (SLANG_FAILED(slangModule->getDefinedEntryPoint(index, entryPointsList[index].writeRef())))
			m_logError("can't get entry point.\n");
	}

	m_log(std::format("Entry point count: {}", entryPointCount));

	if (entryPointsList.empty())
	{
		m_logError("No entry points\n");
		return Binary{};
	}

	//---------------------------------------
	// Compose
	//---------------------------------------
	std::array<slang::IComponentType* const, 2> components = { slangModule, entryPointsList[0] };
	Slang::ComPtr<slang::IComponentType> program;

	if (SLANG_FAILED(m_session->createCompositeComponentType(components.data(), 2, program.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			m_logError(static_cast<const char*>(diagnostics->getBufferPointer()));

		return Binary{};
	}

	//---------------------------------------
	// Link
	//---------------------------------------
	Slang::ComPtr<slang::IComponentType> linkedProgram;
	if (SLANG_FAILED(program->link(linkedProgram.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			m_logError(static_cast<const char*>(diagnostics->getBufferPointer()));

		return Binary{};
	}

	//---------------------------------------
	// Generate SPIR-V
	//---------------------------------------
	Slang::ComPtr<slang::IBlob> spirv;
	if (SLANG_FAILED(linkedProgram->getEntryPointCode(0, 0, spirv.writeRef(), diagnostics.writeRef())))
	{
		if (diagnostics)
			m_logError(static_cast<const char*>(diagnostics->getBufferPointer()));

		return Binary{};
	}

	m_log(std::format("{} successfully compiled", a_filename));

	Binary bin(spirv->getBufferSize());
	memcpy(bin.data(), spirv->getBufferPointer(), spirv->getBufferSize());
	return bin;
}