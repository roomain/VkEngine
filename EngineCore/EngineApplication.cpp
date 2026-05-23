#include "pch.h"
#include <format>
#include <ranges>
#include "EngineApplication.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"
#include "Reflective.h"
#include "CheckParameters.h"

VulkanCapabilities EngineApplication::hostCapabilities()
{
	VulkanCapabilities capabilities;
	auto appInfo = initApplicationInfo("Host capabilities",
		EngineApplication::ENGINE_NAME,
		0,
		EngineApplication::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);

	instanceInfo.enabledLayerCount = 0;
	instanceInfo.enabledExtensionCount = 0;
	VkInstance instance;
	VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &instance))
	getVulkanCapabilities(capabilities, instance);
	vkDestroyInstance(instance, nullptr);
	return capabilities;
}

EngineApplication::EngineApplication(const EngineApplicationParameters& a_appParameters)
{
	// loading modules

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
			auto cleanedLayers = removeDoubloon(parameters.layers);
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(cleanedLayers.size());
			auto tempLayers = vStringToChar(cleanedLayers);
			instanceInfo.ppEnabledLayerNames = tempLayers.data();
			auto cleanedExtensions = removeDoubloon(parameters.extensions);
			instanceInfo.enabledExtensionCount = static_cast<uint32_t>(cleanedExtensions.size());
			auto tempExt = vStringToChar(cleanedExtensions);
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
	{
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_capabilities.instance, "vkDestroyDebugUtilsMessengerEXT");
		vkDestroyDebugUtilsMessengerEXT(m_capabilities.instance, m_debugMessenger, nullptr);
	}
	
	if (m_capabilities.instance)
		vkDestroyInstance(m_capabilities.instance, nullptr);
}

std::vector<DeviceConfiguration> EngineApplication::suitableDevices(const DeviceParameters& a_parameters, const VkSurfaceKHR* a_surface)const
{
	return findSuitableDevices(a_parameters, m_capabilities, a_surface);
}

EngineDevicePtr EngineApplication::createDevice(const DeviceConfiguration& a_configuration)
{
	if (auto iter = std::ranges::find_if(m_deviceInstance, [&a_configuration](const auto& device)
		{
			return device->deviceIndex() == a_configuration.deviceIndex;
		}); iter != m_deviceInstance.cend())
	{
		return *iter;
	}
	else
	{
		DeviceContext ctx
		{
			.m_vkInstance = m_capabilities.instance,
			.m_vkPhysDevice = m_capabilities.devices[a_configuration.deviceIndex].physDevice
		};

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;
		std::vector<std::vector<float>> priorities;
		for (const auto& queueConf : a_configuration.queues)
		{
			priorities.emplace_back(std::vector<float>(queueConf.queueCount, queueConf.priority));
			queueCreateInfo.emplace_back(initQueueCreateInfo(queueConf.familyIndex, 
				queueConf.queueCount, priorities.back(), queueConf.flags));
		}

		VkDeviceCreateInfo createInfo = initDeviceCreateInfo(queueCreateInfo, &a_configuration.features, 0);
		auto cleanedExtensions = removeDoubloon(a_configuration.extensions);
		auto tempExtension = vStringToChar(cleanedExtensions);
		auto cleanedLayers = removeDoubloon(a_configuration.layers);
		auto tempLayers = vStringToChar(cleanedLayers);

		createInfo.ppEnabledExtensionNames = tempExtension.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(cleanedExtensions.size());
		createInfo.ppEnabledLayerNames = tempLayers.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(cleanedLayers.size());

		// parameter of extensions

		// todo

		vkCreateDevice(m_capabilities.devices[a_configuration.deviceIndex].physDevice, &createInfo, nullptr, &ctx.m_vkDevice);
		EngineDevicePtr newDevice (new EngineDevice(a_configuration, ctx));
		m_deviceInstance.emplace_back(newDevice);
		return newDevice;
	}
}

EngineRendererPtr EngineApplication::createRenderer(const RendererConfiguration& a_configuration)
{
	return EngineRendererPtr(new EngineRenderer(createDevice(a_configuration.deviceConf), a_configuration.surfaceConf));
}