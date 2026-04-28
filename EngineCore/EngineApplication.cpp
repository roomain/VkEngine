#include "pch.h"
#include <format>
#include "EngineApplication.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"
#include "Reflective.h"
#include "EngineParameters.h"
#include "CheckParameters.h"

EngineApplication::EngineApplication(const EngineApplicationParameters& a_appParameters)
{
	auto appInfo = initApplicationInfo(a_appParameters.appName, 
		EngineApplication::ENGINE_NAME, 
		a_appParameters.appVersion,
		EngineApplication::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);

	Reflective::setLogCallback(static_cast<LogCallback>(&EngineLog::reflectLog));
	if (Reflective::instance().loadFile(a_appParameters.parametersFilename))
	{
		if (Reflective::instance().hasProfile(a_appParameters.parametersProfile))
		{
			EngineParameters parameters;
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

std::vector<uint32_t> EngineApplication::suitableDevices(const DeviceParameters& a_parameters, const VkSurfaceKHR* a_surface)const
{
	return findSuitableDevices(a_parameters, m_capabilities, a_surface);
}

EngineDevicePtr EngineApplication::createDevice(const DeviceParameters& a_parameters, const uint32_t a_deviceIndex)
{
	//
	//return new EngineDevice(chosenDev, const DeviceContext & a_ctx)
}

EngineRendererPtr EngineApplication::createRenderer(const RendererParameters& a_parameters)
{
	//return new EngineRenderer()
}