#pragma once
/***********************************************
* @headerfile Reflection_enumdefs.h
* @date 26 / 03 / 2026
* @author Roomain
************************************************/
#include <string_view>
#include <unordered_map>
#include <glaze/glaze.hpp>

#define REFLECT_BEGIN_ENUM(Type) \
template <>                                      \
struct glz::meta<Type> {                         \
    static constexpr auto value = glz::enumerate(\

#define REFLECT_ENUM(name, value) \
#name, value,

#define REFLECT_END_ENUM(name, value) \
#name, value \
    ); \
};
