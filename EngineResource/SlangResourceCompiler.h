#pragma once
/***********************************************
* @headerfile SlangResourceCompiler.h
* @date 23 / 08 / 2026
* @author Roomain
************************************************/
#include <string>
#include <slang.h>
#include <slang-com-ptr.h>
#include "Compiler.h"

/*@brief slang shader compiler : compile to spirv*/
class SlangResourceCompiler : public Compiler
{
private:
	Slang::ComPtr<slang::IGlobalSession> m_globalSession;
	Slang::ComPtr<slang::ISession> m_session;


public:
	static bool isFileAvailable(const std::string& a_filename);
	SlangResourceCompiler();
	virtual ~SlangResourceCompiler();
	NOT_COPIABLE(SlangResourceCompiler)
	NOT_MOVABLE(SlangResourceCompiler)

		std::expected<Binary, CompilerError> compile(const std::string& a_filename) override;
};

