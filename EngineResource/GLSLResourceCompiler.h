#pragma once
/***********************************************
* @headerfile GLSLResourceCompiler.h
* @date 29 / 08 / 2026
* @author Roomain
************************************************/
#include "Compiler.h"

class GLSLResourceCompiler : public Compiler
{
private:
	bool m_bIsInittialized = false;
	static int getShaderTypeFromFile(const std::string& a_filename);

public:
	static bool isFileAvailable(const std::string& a_filename);
	GLSLResourceCompiler();
	virtual ~GLSLResourceCompiler();
	NOT_COPIABLE(GLSLResourceCompiler)
	NOT_MOVABLE(GLSLResourceCompiler)

	std::expected<Binary, CompilerError> compile(const std::string& a_filename) override;
};

