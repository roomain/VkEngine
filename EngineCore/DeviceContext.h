#pragma once
/***********************************************
* @headerfile DeviceContext.h
* @date 19 / 03 / 2026
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

/*@brief base data use for vulkan call*/
struct DeviceContext
{
	VkInstance vkInstance = VK_NULL_HANDLE;			/*!< vulkan instance*/
	VkPhysicalDevice vkPhysDevice = VK_NULL_HANDLE;	/*!< vulkan physical device*/
	VkDevice vkDevice = VK_NULL_HANDLE;				/*!< vulkan logical device corresponding to physical device*/
	VmaAllocator memAllocator = VK_NULL_HANDLE;		/*!< device memory allocation*/
};
