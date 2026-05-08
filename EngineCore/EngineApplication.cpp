#include "pch.h"
#include <format>
#include <ranges>
#include "EngineApplication.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"
#include "Reflective.h"
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

std::vector<DeviceConfiguration> EngineApplication::suitableDevices(const DeviceParameters& a_parameters, const VkSurfaceKHR* a_surface)const
{
	return findSuitableDevices(a_parameters, m_capabilities, a_surface);
}

EngineDevicePtr EngineApplication::createDevice(const DeviceConfiguration& a_parameters, const uint32_t a_deviceIndex)
{
	if (auto iter = std::ranges::find_if(m_deviceInstance, [a_deviceIndex](const auto& device)
		{
			return device->deviceIndex() == a_deviceIndex;
		}); iter != m_deviceInstance.cend())
	{
		return *iter;
	}
	else
	{
		DeviceContext ctx
		{
			.m_vkInstance = m_capabilities.instance,
			.m_vkPhysDevice = m_capabilities.devices[a_deviceIndex].physDevice
		};

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
		std::vector<std::vector<float>> priorities;
		for (const auto& queueConf : a_parameters.queues)
		{
			priorities.emplace_back(std::vector<float>(queueConf.queueCount, queueConf.priority));
			queueCreateInfo.emplace_back(initQueueCreateInfo(queueConf.familyIndex, 
				queueConf.queueCount, priorities.back(), queueConf.flags));
		}

		VkDeviceCreateInfo createInfo = initDeviceCreateInfo(queueCreateInfo, &a_parameters.features, 0);
		auto tempExtension = vStringToChar(a_parameters.extensions);
		auto tempLayers = vStringToChar(a_parameters.layers);

		createInfo.ppEnabledExtensionNames = tempExtension.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(a_parameters.extensions.size());
		createInfo.ppEnabledLayerNames = tempLayers.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(a_parameters.layers.size());
		vkCreateDevice(m_capabilities.devices[a_deviceIndex].physDevice, &createInfo, nullptr, &ctx.m_vkDevice);
		EngineDevicePtr newDevice (new EngineDevice(a_parameters, ctx));
		m_deviceInstance.emplace_back(newDevice);
		return newDevice;
	}
}

EngineRendererPtr EngineApplication::createRenderer(const DeviceConfiguration& a_parameters)
{
	//return new EngineRenderer()
}