#include "pch.h"
#include "EngineApplication.h"

EngineApplication::EngineApplication(const std::string& a_appName, const uint32_t a_appVersion)
{
	auto appInfo = initApplicationInfo(a_appName, EngineApplication::ENGINE_NAME, a_appVersion,
		EngineApplication::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);
	//todo set layers and extensions
	//vkCreateInstance(&instanceInfo)
}