#include "pch.h"
#include "EngineExceptions.h"

EngineException::EngineException(const std::source_location a_location, const char* a_what) :
	std::exception{ a_what }, m_location{ a_location }
{
	EngineLog::error("{} {} line {}: {}", a_location.file_name(), a_location.function_name(), a_location.line(), a_what);
}

EngineVulkanException::EngineVulkanException(const std::source_location a_location, const char* a_what) :
	EngineException{ a_location, a_what }
{
	//
}

EngineManageException::EngineManageException(const std::source_location a_location, const char* a_what) :
	EngineException{ a_location, a_what }
{
	//
}