#pragma once
/***********************************************
* @headerfile Compiler.h
* @date 29 / 08 / 2026
* @author Roomain
************************************************/
#include <string>
#include <expected>
#include <format>
#include "Resources.h"
#include "notcopiable.h"

struct CompilerError
{
	enum class Type
	{
		InitError,
		WrongExtension,
		CompileError,
		LinkError,
		SpirvGen
	};

	Type m_errorType;
	std::string m_info;

	std::string errorMessage()const
	{
		switch (m_errorType)
		{
		case Type::InitError:
			return std::format("Compiler init error: {}", m_info);
		case Type::WrongExtension:
			return std::format("Wrong file extension: {}", m_info);
		case Type::CompileError:
			return std::format("Compile error: {}", m_info);
		case Type::LinkError:
			return std::format("Link error: {}", m_info);
		case Type::SpirvGen:
			return std::format("Spirv Generation: {}", m_info);
		}
		return "";
	}
};

/*@brief interface for compiler*/
class Compiler
{
public:
	Compiler() = default;
	virtual ~Compiler() = default;
	NOT_COPIABLE(Compiler)
	NOT_MOVABLE(Compiler)
	virtual std::expected<Binary, CompilerError> compile(const std::string& a_filename) = 0;
};

