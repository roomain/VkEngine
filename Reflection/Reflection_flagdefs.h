#pragma once
/***********************************************
* @headerfile Reflection_flagdefs.h
* @date 26 / 03 / 2026
* @author Roomain
************************************************/
#include <string_view>
#include <unordered_map>
#include <glaze/glaze.hpp>

#define REFLECT_BEGIN_FLAG(flagbit, flag)												\
template<>																		\
struct glz::from<glz::JSON, flag>												\
{																				\
	static inline const std::unordered_map<std::string_view, flagbit> flag##_map{\

#define REFLECT_FLAG(flagbit)	\
	{#flagbit, flagbit},\

#define REFLECT_END_FLAG(flag)                                                                                  \
};                                                                                                      \
template <auto Opts>                                                                                    \
static void op(flag& value, is_context auto&& ctx, auto&& it, auto&& end) {                             \
    std::string s;                                                                                      \
    parse<JSON>::op<Opts>(s, ctx, it, end);                                                             \
    value = static_cast<flag>(0);                                                                       \
    std::stringstream strStream(s);                                                                     \
    std::string token;                                                                                  \
                                                                                                        \
    while (std::getline(strStream, token, '|'))                                                         \
    {                                                                                                   \
        const auto blankPosBegin = token.find_first_not_of(" \t");                                      \
        if (blankPosBegin == std::string::npos) continue;                                               \
        const auto blankPosEnd = token.find_last_not_of(" \t");                                         \
                                                                                                        \
        const std::string_view trimmed{ token.data() + blankPosBegin, blankPosEnd - blankPosBegin + 1 };\
                                                                                                        \
        if (auto iter = flag##_map.find(trimmed); iter != flag##_map.cend())                            \
        {                                                                                               \
            value = static_cast<flag>(                                                                  \
                static_cast<uint32_t>(value) | static_cast<uint32_t>(val));                             \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            ctx.error = glz::error_code::unexpected_enum;                                               \
            return;                                                                                     \
        }                                                                                               \
    }                                                                                                   \
}                                                                                                       \
};                                                                                                      \

