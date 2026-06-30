#pragma once
/***********************************************
* @headerfile reflective_convert_from_string.h
* @date 30 / 06 / 2026
* @author Roomain
************************************************/

#include <string_view>
#include "string_utils.h"

#define BEGIN_ENUM(Type) \
template<> \
[[nodiscard]] inline Type convert<Type, std::string_view>(const std::string_view& data) \
{ \
    Type flag = static_cast<Type>(0); \
    auto orOperator = [](const Type first, const Type& second)                                          \
    {                                                                                                   \
        return static_cast<Type>(static_cast<unsigned int>(first) | static_cast<unsigned int>(second)); \
    };                                                                                                  \
    const auto splitted = split(std::string(data), '|'); \
    for (const auto& local_data : splitted) \
    { \

#define VALUE_ENUM(value) \
        if(trim(local_data) == #value) \
            flag = orOperator(flag, value); \

#define END_ENUM \
    } \
    return flag; \
}
