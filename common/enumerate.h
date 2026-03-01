/***********************************************
* @headerfile enumerate.cppm
* @date 19 / 02 / 2026
* @author Roomain
************************************************/
#pragma once

#include <cstdint>
#include <vector>

/*@brief Tool for vulkan enumeration simplification*/
template<typename EnumFunction, typename EnumDataType, typename ...EnumArgs>
void enumerate(const EnumFunction& a_enumFun, std::vector<EnumDataType>& a_enumDataList, EnumArgs... a_enumArgs)
{
    uint32_t enumCount = 0;
    a_enumFun(a_enumArgs..., &enumCount, nullptr);
    a_enumDataList.resize(enumCount);
    a_enumFun(a_enumArgs..., &enumCount, a_enumDataList.data());
}

template<typename EnumFunction, typename EnumDataType, typename TypeRet, typename ...EnumArgs>
[[nodiscard]] TypeRet enumerateEx(const EnumFunction& a_enumFun, std::vector<EnumDataType>& a_enumDataList, const TypeRet a_expected, EnumArgs... a_enumArgs)
{
    uint32_t enumCount = 0;
    TypeRet result = a_enumFun(a_enumArgs..., &enumCount, nullptr);
    if(a_expected == result)
    {
        a_enumDataList.resize(enumCount);
        return a_enumFun(a_enumArgs..., &enumCount, a_enumDataList.data());
    }
    return result;
}