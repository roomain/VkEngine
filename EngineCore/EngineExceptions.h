#pragma once
/***********************************************
* @headerfile EngineExceptions.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <string>
#include <exception>
#include <source_location>
#include "VkEnumToString.h"

/*@brief enumerate log severity*/
enum class LogSeverity
{
    None = 0,
    Info = 1,
    Warning = 1 << 1,
    Error = 1 << 2,
    Critical = 1 << 3
};

class EngineException : public std::exception
{
private:
    std::source_location m_location = std::source_location::current();  /*!< log location*/

public:
    EngineException() = delete;
    explicit EngineException(const std::source_location a_location, const char* a_what);
    virtual ~EngineException() = default;
    std::source_location location()const { return m_location; }
};


class EngineVulkanException : public EngineException
{
public:
    EngineVulkanException() = delete;
    explicit EngineVulkanException(const std::source_location a_location, const char* a_what);
    virtual ~EngineVulkanException() = default;
};

class EngineManageException : public EngineException
{
public:
    EngineManageException() = delete;
    explicit EngineManageException(const std::source_location a_location, const char* a_what);
    virtual ~EngineManageException() = default;
};

#define VK_CHECK_EXCEPT(vkCall) \
if (const VkResult result = vkCall; result != VK_SUCCESS) \
	throw EngineVulkanException(std::source_location::current(), (#vkCall##": " + to_string(result)).c_str());

