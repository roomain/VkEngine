#pragma once
/***********************************************
* @headerfile SlangResourceCompiler.h
* @date 23 / 08 / 2026
* @author Roomain
************************************************/
#include <string>
#include <slang.h>
#include <slang-com-ptr.h>
#include "Resources.h"
#include "notcopiable.h"
#include "LogCallback.h"

/*@brief slang shader compiler : compile to spirv*/
class SlangResourceCompiler
{
private:
	Slang::ComPtr<slang::IGlobalSession> m_globalSession;
	Slang::ComPtr<slang::ISession> m_session;
	LogCallback m_log;
	LogCallback m_logError;


public:
	SlangResourceCompiler() = delete;
	explicit SlangResourceCompiler(LogCallback a_log, LogCallback a_errorLog);
	virtual ~SlangResourceCompiler();
	NOT_COPIABLE(SlangResourceCompiler)
	NOT_MOVABLE(SlangResourceCompiler)

	Binary compile(const std::string& a_filename);
};

