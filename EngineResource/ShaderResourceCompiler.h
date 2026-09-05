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
private:
	std::unique_ptr<GLSLResourceCompiler> m_pGlslCompiler;
	std::unique_ptr<SlangResourceCompiler> m_pSlangCompiler;

public:
	ShaderResourceCompiler() = default;
	virtual ~ShaderResourceCompiler() = default;
	std::expected<Binary, CompilerError> compile(const std::string& a_filename)override;
};

