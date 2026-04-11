#include "pch.h"
#include "VulkanCapabilities.h"


void getVulkanInstanceCapabilities(VulkanCapabilities& a_VkCap)
{
	enumerate(&vkEnumerateInstanceExtensionProperties, a_VkCap.extensions, nullptr);
	enumerate(&vkEnumerateInstanceLayerProperties, a_VkCap.layers);
}

void getVulkanCapabilities(VulkanCapabilities& a_VkCap, const VkInstance a_instance)
{
	if(a_VkCap.instance == VK_NULL_HANDLE)
		a_VkCap.instance = a_instance;

	if (a_VkCap.extensions.empty())
		enumerate(&vkEnumerateInstanceExtensionProperties, a_VkCap.extensions, nullptr);

	if (a_VkCap.layers.empty())
		enumerate(&vkEnumerateInstanceLayerProperties, a_VkCap.layers);

	std::vector<VkPhysicalDevice> vDevices;
	enumerate(&vkEnumeratePhysicalDevices, vDevices, a_VkCap.instance);
	for (const auto& physDev : vDevices)
		getDeviceCapabilities(physDev, a_VkCap.devices.emplace_back());
}


void getDeviceCapabilities(const VkPhysicalDevice& a_physDev, VulkanDeviceCapabilities& a_devCap)
{
	a_devCap.physDevice = a_physDev;
	enumerate(&vkEnumerateDeviceExtensionProperties, a_devCap.extensions, a_devCap.physDevice, nullptr);
	enumerate(&vkEnumerateDeviceLayerProperties, a_devCap.layers, a_devCap.physDevice);
	vkGetPhysicalDeviceProperties(a_devCap.physDevice, &a_devCap.properties);
	vkGetPhysicalDeviceFeatures(a_devCap.physDevice, &a_devCap.features);

	vkGetPhysicalDeviceMemoryProperties(a_devCap.physDevice, &a_devCap.memoryProperties);
}

void getQueueFamiliesCapabilities(const VkPhysicalDevice& a_physDev, std::vector<VkQueueFamilyProperties>& a_queueFamilies)
{
	enumerate(&vkGetPhysicalDeviceQueueFamilyProperties, a_queueFamilies, a_physDev);
}