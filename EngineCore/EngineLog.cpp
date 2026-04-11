#include "pch.h"
#include <filesystem>
#include "EngineLog.h"
#include <boost/log/utility/setup.hpp>

std::string EngineLog::s_logFile;

bool EngineLog::initLog(const std::string& a_logFilePath)
{
	initLog();
	std::filesystem::path path = std::filesystem::path(a_logFilePath).parent_path();
	if (std::filesystem::exists(path))
	{
		s_logFile = a_logFilePath;
		
		boost::log::add_file_log(
			boost::log::keywords::file_name = std::format("{}_%3N.log", s_logFile),
			boost::log::keywords::format = COMMON_FMT,
			boost::log::keywords::auto_flush = true
		);

		return true;
	}
	return false;
}

void EngineLog::initLog()
{
	boost::log::add_console_log(
		std::cout,
		boost::log::keywords::format = COMMON_FMT,
		boost::log::keywords::auto_flush = true
	);
	boost::log::add_common_attributes();
}

VkBool32 EngineLog::vulkanDebug(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT /*a_messageTypes*/,
	const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData,
	void*)
{
	switch (a_messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		EngineLog::info("vulkan info: {} ", a_pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		EngineLog::warning("vulkan warning: {} ", a_pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		EngineLog::error("vulkan error: {} ", a_pCallbackData->pMessage);
		break;
	default:
		break;
	}
	return true;
}

void EngineLog::reflectLog(const std::source_location& a_loc, const std::string_view message)
{
	EngineLog::info("File {} - line {}: {}", a_loc.file_name(), a_loc.line(), message);
}