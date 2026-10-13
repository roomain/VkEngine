#pragma once
/***********************************************
* @headerfile ShaderResourceCompiler.h
* @date 02 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "Compiler.h"

class GLSLResourceCompiler;
class SlangResourceCompiler;

class ShaderResourceCompiler : public Compiler
{

public:
	ShaderResourceCompiler();
	~ShaderResourceCompiler()final;
	std::expected<Binary, CompilerError> compile(const std::string& a_filename)override;

private:
	/*Must be declared after ctor for forwar declare*/
	std::unique_ptr<GLSLResourceCompiler> m_pGlslCompiler;
	std::unique_ptr<SlangResourceCompiler> m_pSlangCompiler;
};

