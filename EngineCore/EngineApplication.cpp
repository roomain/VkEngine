#include "pch.h"
#include <format>
#include "EngineApplication.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"
#include "ReflectionReader.h"
#include "EngineParameters.h"

EngineApplication::EngineApplication(const EngineApplicationParameters& a_appParameters)
{
	auto appInfo = initApplicationInfo(a_appParameters.appName, 
		EngineApplication::ENGINE_NAME, 
		a_appParameters.appVersion,
		EngineApplication::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);

	ReflectionReader::setLogCallback(static_cast<LogCallback>(&EngineLog::reflectLog));
	if (ReflectionReader::instance().loadFile(a_appParameters.parametersFilename))
	{
		if (ReflectionReader::instance().hasProfile(a_appParameters.parametersProfile))
		{
			EngineParameters parameters;
			ReflectionReader::instance().deserialize("EngineParameters", parameters);
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(parameters.layers.size());
			auto tempLayers = vStringToChar(parameters.layers);
			instanceInfo.ppEnabledLayerNames = tempLayers.data();
			instanceInfo.enabledExtensionCount = static_cast<uint32_t>(parameters.extensions.size());
			auto tempExt = vStringToChar(parameters.extensions);
			instanceInfo.ppEnabledExtensionNames = tempExt.data();
			VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &m_capabilities.instance))
			getVulkanCapabilities(m_capabilities);
			if (parameters.debugging)
			{
				auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_capabilities.instance, "vkCreateDebugUtilsMessengerEXT");
				if (vkCreateDebugUtilsMessengerEXT)
				{
					VkDebugUtilsMessengerCreateInfoEXT messCI = initMessageCallbackCreateInfo();
					messCI.pUserData = this;
					messCI.pfnUserCallback = &EngineLog::vulkanDebug;
					VK_CHECK_EXCEPT(vkCreateDebugUtilsMessengerEXT(m_capabilities.instance, &messCI, nullptr, &m_debugMessenger))
				}
			}
		}
		else
		{
			EngineLog::error("Can't find profile in file {}", a_appParameters.parametersProfile);
		}
	}
	else
	{
		EngineLog::error("Can't read parameter files");
	}
}

EngineApplication::~EngineApplication()
{
	if (m_debugMessenger)
		vkDestroyDebugUtilsMessengerEXT(m_capabilities.instance, m_debugMessenger, nullptr);
	
	if (m_capabilities.instance)
		vkDestroyInstance(m_capabilities.instance, nullptr);
}

EngineDevicePtr EngineApplication::createDevice(const DeviceParameters& a_parameters)
{
	//
}

EngineRendererPtr EngineApplication::createRenderer(const RendererParameters& a_parameters)
{
	//
}