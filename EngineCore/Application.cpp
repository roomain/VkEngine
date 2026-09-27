#include "pch.h"
#include <format>
#include <ranges>
#include "Application.h"
#include "Renderer.h"
#include "Device.h"
#include "Reflective.h"
#include "CheckParameters.h"

VulkanCapabilities Application::hostCapabilities()
{
	VulkanCapabilities capabilities;
	auto appInfo = initApplicationInfo("Host capabilities",
		Application::ENGINE_NAME,
		0,
		Application::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);

	instanceInfo.enabledLayerCount = 0;
	instanceInfo.enabledExtensionCount = 0;
	VkInstance instance;
	VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &instance))
	getVulkanCapabilities(capabilities, instance);
	vkDestroyInstance(instance, nullptr);
	// reset
	capabilities.instance = VK_NULL_HANDLE;
	return capabilities;
}

Application::Application(const ApplicationParameters& a_appParameters)
{
	auto appInfo = initApplicationInfo(a_appParameters.appName, 
		Application::ENGINE_NAME, 
		a_appParameters.appVersion,
		Application::ENGINE_VERSION);
	auto instanceInfo = initInstanceCreateInfo(&appInfo);

	Reflective::setLogCallback(static_cast<LogCallback>(&Log::reflectLog));
	if (Reflective::instance().loadFile(a_appParameters.parametersFilename))
	{
		if (Reflective::instance().hasProfile(a_appParameters.parametersProfile))
		{
			Reflective::instance().setCurrentProfile(a_appParameters.parametersProfile);
			// setup vulkan instance parameters
			EngineParameters parameters;
			auto cleanedLayers = removeDoubloon(parameters.layers);
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(cleanedLayers.size());
			auto tempLayers = vStringToChar(cleanedLayers);
			instanceInfo.ppEnabledLayerNames = tempLayers.data();
			auto cleanedExtensions = removeDoubloon(parameters.extensions);
			instanceInfo.enabledExtensionCount = static_cast<uint32_t>(cleanedExtensions.size());
			auto tempExt = vStringToChar(cleanedExtensions);
			instanceInfo.ppEnabledExtensionNames = tempExt.data();
			// create vulkan instance
			VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &m_capabilities.instance))

			// setup vulkan capabilities
			getVulkanCapabilities(m_capabilities);
			if (parameters.debugging)
			{
				auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_capabilities.instance, "vkCreateDebugUtilsMessengerEXT");
				if (vkCreateDebugUtilsMessengerEXT)
				{
					VkDebugUtilsMessengerCreateInfoEXT messCI = initMessageCallbackCreateInfo();
					messCI.pUserData = this;
					messCI.pfnUserCallback = &Log::vulkanDebug;
					VK_CHECK_EXCEPT(vkCreateDebugUtilsMessengerEXT(m_capabilities.instance, &messCI, nullptr, &m_debugMessenger))
				}
			}
		}
		else
		{
			Log::error("Can't find profile in file {}", a_appParameters.parametersProfile);
		}
	}
	else
	{
		Log::error("Can't read parameter files");
	}
}

Application::~Application()
{
	if (m_debugMessenger)
	{
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_capabilities.instance, "vkDestroyDebugUtilsMessengerEXT");
		vkDestroyDebugUtilsMessengerEXT(m_capabilities.instance, m_debugMessenger, nullptr);
	}
	
	if (m_capabilities.instance)
		vkDestroyInstance(m_capabilities.instance, nullptr);
}

std::vector<DeviceConfiguration> Application::suitableDevices(const DeviceParameters& a_parameters, const VkSurfaceKHR* a_surface)const
{
	return findSuitableDevices(a_parameters, m_capabilities, a_surface);
}

DevicePtr Application::createDevice(const DeviceConfiguration& a_configuration, bool a_enableDynRendering)
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
			.vkInstance = m_capabilities.instance,
			.vkPhysDevice = m_capabilities.devices[a_configuration.deviceIndex].physDevice
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

		if (a_enableDynRendering)
		{
			VkPhysicalDeviceDynamicRenderingFeaturesKHR enabledDynamicRenderingFeaturesKHR = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
				.dynamicRendering = VK_TRUE
			};

			VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
				.pNext = &enabledDynamicRenderingFeaturesKHR,
				.features = a_configuration.features
			};
			createInfo.pEnabledFeatures = nullptr;
			createInfo.pNext = &physicalDeviceFeatures2;
		}

		vkCreateDevice(m_capabilities.devices[a_configuration.deviceIndex].physDevice, &createInfo, nullptr, &ctx.vkDevice);
		DevicePtr newDevice (new Device(a_configuration, ctx));
		m_deviceInstance.emplace_back(newDevice);
		return newDevice;
	}
}

RendererPtr Application::createRenderer(const RendererConfiguration& a_configuration)
{
	return RendererPtr(new Renderer(createDevice(a_configuration.deviceConf, true), a_configuration.surfaceConf));
}