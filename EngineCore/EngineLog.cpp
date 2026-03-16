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