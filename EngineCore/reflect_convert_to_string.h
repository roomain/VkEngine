#pragma once
/***********************************************
* @headerfile reflective_convert_to_string.h
* @date 30 / 06 / 2026
* @author Roomain
************************************************/
#include <string>

#define BEGIN_ENUM(Type) \
template<> \
[[nodiscard]] inline std::string convert<std::string, Type>(const Type& data) \
{ \
    std::string str;


#define VALUE_ENUM(value) \
        if((static_cast<unsigned int>(data) & static_cast<unsigned int>(value)) == static_cast<unsigned int>(value))  \
        { \  
            if (!str.empty()) \
            { \
                str += "|"; \
            } \
            \
            str += #value; \
        }

#define END_ENUM \
    return str; \
}
