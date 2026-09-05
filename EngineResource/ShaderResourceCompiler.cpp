#include "ShaderResourceCompiler.h"
#include "GLSLResourceCompiler.h"
#include "SlangResourceCompiler.h"

std::expected<Binary, CompilerError> ShaderResourceCompiler::compile(const std::string& a_filename)
{
	if (GLSLResourceCompiler::isFileAvailable(a_filename))
	{
		return m_pGlslCompiler->compile(a_filename);
	}
	else if (SlangResourceCompiler::isFileAvailable(a_filename))
	{
		return m_pSlangCompiler->compile(a_filename);
	}
	return Binary{};
}