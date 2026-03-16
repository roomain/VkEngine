#pragma once
/***********************************************
* @headerfile EngineLog.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <string>
#include <format>
#include <boost/log/trivial.hpp>
#include "VkEnumToString.h"

/*@brief Log configuration*/
class EngineLog
{
private:
	static std::string s_logFile;
	static inline const std::string COMMON_FMT = "[%TimeStamp%][%Severity%]:  %Message%";
public:
	/*@brief set log file return false if can't create file*/
	static [[nodiscard]] bool initLog(const std::string& a_logFilePath);
	static void initLog();
	
	template<typename... Args>
	static void log(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(trace) << std::format(fmt, std::forward<Args>(data)...);
	}

	template<typename... Args>
	static void debug(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(debug) << std::format(fmt, std::forward<Args>(data)...);
	}

	template<typename... Args>
	static void info(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(info) << std::format(fmt, std::forward<Args>(data)...);
	}

	template<typename... Args>
	static void warning(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(warning) << std::format(fmt, std::forward<Args>(data)...);
	}

	template<typename... Args>
	static void error(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(error) << std::format(fmt, std::forward<Args>(data)...);
	}

	template<typename... Args>
	static void critical(std::format_string<Args...> fmt, Args&&... data)
	{
		BOOST_LOG_TRIVIAL(fatal) << std::format(fmt, std::forward<Args>(data)...);
	}
};

#define VK_CHECK_LOG(vkCall) \
if (const VkResult result = vkCall; result != VK_SUCCESS) \
	EngineLog::warning(#vkCall##": " + to_string(result));