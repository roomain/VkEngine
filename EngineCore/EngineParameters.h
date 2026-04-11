#pragma once
/***********************************************
* @headerfile EngineParameters.h
* @date 28 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <string>

struct BaseParameters
{
	std::vector<std::string> extensions;
	std::vector<std::string> layers;
};

struct EngineParameters : BaseParameters
{
	bool debugging = false;
};


struct DeviceParameters : BaseParameters
{
	//
};