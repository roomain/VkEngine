#pragma once
/***********************************************
* @headerfile VulkanCapabilities.h
* @date 20 / 02 / 2026
* @author Roomain
************************************************/

#include <vector>
#include <vulkan/vulkan.hpp>

struct BaseVulkanCapabilities
{
	std::vector<VkExtensionProperties> extensions;  /*!< extension properties*/
	std::vector<VkLayerProperties> layers;          /*!< layer properties*/
};

struct VulkanDeviceCapabilities : BaseVulkanCapabilities
{
	VkPhysicalDevice physDevice = VK_NULL_HANDLE;		/*!< physical device handle*/
	VkPhysicalDeviceProperties properties;				/*!< properties of the device*/
	VkPhysicalDeviceFeatures features;					/*!< features supported by device*/
	VkPhysicalDeviceMemoryProperties memoryProperties;	/*!< device memory properties*/
	std::vector<VkQueueFamilyProperties> queueFamilies; /*!< queue families properties for the device*/
};

struct VulkanCapabilities : BaseVulkanCapabilities
{
	VkInstance instance = VK_NULL_HANDLE;
	std::vector<VulkanDeviceCapabilities> devices;
};

void getVulkanInstanceCapabilities(VulkanCapabilities& a_VkCap);
void getVulkanCapabilities(const VkInstance& a_instance, VulkanCapabilities& a_VkCap);
void getDeviceCapabilities(const VkPhysicalDevice& a_physDev, VulkanDeviceCapabilities& a_devCap);
void getQueueFamiliesCapabilities(const VkPhysicalDevice& a_physDev, std::vector<VkQueueFamilyProperties>& a_queueFamilies);
