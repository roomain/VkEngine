#include "pch.h"
#include "Exceptions.h"

Exception::Exception(const std::source_location a_location, const char* a_what) :
	std::exception{ a_what }, m_location{ a_location }
{
	Log::error("{} {} line {}: {}", a_location.file_name(), a_location.function_name(), a_location.line(), a_what);
}

VulkanException::VulkanException(const std::source_location a_location, const char* a_what) :
	Exception{ a_location, a_what }
{
	//
}

ManageException::ManageException(const std::source_location a_location, const char* a_what) :
	Exception{ a_location, a_what }
{
	//
}