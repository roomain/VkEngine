#pragma once
/***********************************************
* @headerfile EngineParameters.h
* @date 28 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <string>
#include "Reflective_macros.h"

REFLECT_CLASS
struct BaseParameters
{
	std::vector<std::string> extensions;
	std::vector<std::string> layers;
	REFLECT_DEFINE(BaseParameters);
};
REFLECT_IMPL(BaseParameters)

REFLECT_CLASS
struct EngineParameters : BaseParameters
{
	bool debugging = false;
	REFLECT_DEFINE(EngineParameters);
};
REFLECT_IMPL(EngineParameters)

REFLECT_CLASS
struct DeviceParameters : BaseParameters
{
	//
	REFLECT_DEFINE(DeviceParameters);
};
REFLECT_IMPL(DeviceParameters)