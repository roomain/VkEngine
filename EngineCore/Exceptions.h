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
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )

/*@brief enumerate log severity*/
enum class LogSeverity
{
    None = 0,
    Info = 1,
    Warning = 1 << 1,
    Error = 1 << 2,
    Critical = 1 << 3
};

class ENGINECORE_EXPORT Exception : public std::exception
{
private:
    std::source_location m_location = std::source_location::current();  /*!< log location*/

public:
    Exception() = delete;
    explicit Exception(const std::source_location a_location, const char* a_what);
    ~Exception()override = default;
    std::source_location location()const { return m_location; }
};


class ENGINECORE_EXPORT VulkanException : public Exception
{
public:
    VulkanException() = delete;
    explicit VulkanException(const std::source_location a_location, const char* a_what);
    ~VulkanException()override = default;
};

class ENGINECORE_EXPORT ManageException : public Exception
{
public:
    ManageException() = delete;
    explicit ManageException(const std::source_location a_location, const char* a_what);
    ~ManageException()override = default;
};

#define VK_CHECK_EXCEPT(vkCall) \
if (const VkResult result = vkCall; result != VK_SUCCESS) \
	throw VulkanException(std::source_location::current(), (#vkCall##": " + to_string(result)).c_str());

#pragma warning(pop)
