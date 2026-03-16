#pragma once
/***********************************************
* @headerfile EngineSwapChain.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <vulkan/vulkan.hpp>
#include "DeviceContext.h"

class EngineSwapChain
{
private:

	struct SwapChainFrame
	{
		VkImage m_image = VK_NULL_HANDLE;			/*!< image buffer*/
		VkImageView m_imageView = VK_NULL_HANDLE;	/*!< image view*/
	};

	DeviceContext m_deviceCtx;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;		/*!< vulkan surface*/
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;	/*!< vulkan swapchain*/

	// swap chain saved capability for resize
	VkSurfaceCapabilitiesKHR m_surfCaps;			/*!< surface capabilities*/
	VkSwapchainCreateInfoKHR m_swapchainCI;			/*!< swap chain create Info*/

	std::vector<SwapChainFrame> m_frames;			/*!< frames*/

	static [[nodiscard]] uint32_t getImageCount(const VkSurfaceCapabilitiesKHR& a_surfCaps);
	static [[nodiscard]] VkSurfaceFormatKHR findSurfaceFormat(const VkPhysicalDevice& a_physDev, const VkSurfaceKHR a_surf);
	static [[nodiscard]] VkCompositeAlphaFlagBitsKHR findCompositeAlpha(const VkSurfaceCapabilitiesKHR& a_surfCaps);
	static [[nodiscard]] VkExtent2D getImageExtent(const VkSurfaceCapabilitiesKHR& a_surfCaps, const uint32_t a_width, const uint32_t a_height);
	void createImageBuffers();
	void releaseSwapchain(VkSwapchainKHR a_oldSwapChain);
	void createSwapChain(const uint32_t a_width, const uint32_t a_height);

public:
	void resize(const uint32_t a_width, const uint32_t a_height);
};

