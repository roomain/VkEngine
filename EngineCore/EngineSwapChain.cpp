#include "pch.h"
#include "EngineSwapChain.h"

uint32_t EngineSwapChain::getImageCount(const VkSurfaceCapabilitiesKHR& a_surfCaps)
{
	uint32_t desiredNumberOfSwapchainImages = a_surfCaps.minImageCount + 1;
	if ((a_surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > a_surfCaps.maxImageCount))
	{
		desiredNumberOfSwapchainImages = a_surfCaps.maxImageCount;
	}
	return desiredNumberOfSwapchainImages;
}

VkSurfaceFormatKHR EngineSwapChain::findSurfaceFormat(const VkPhysicalDevice& a_physDev, const VkSurfaceKHR a_surf)
{
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	enumerate(&vkGetPhysicalDeviceSurfaceFormatsKHR, surfaceFormats, a_physDev, a_surf);

	VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];

	// format un preferential order
	const std::vector<VkFormat> preferredImageFormats = {
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_A8B8G8R8_UNORM_PACK32
	};

	// find compatible format
	for (const auto& availableFormat : surfaceFormats) {
		if (std::ranges::find(preferredImageFormats, availableFormat.format) != preferredImageFormats.end()) {
			surfaceFormat = availableFormat;
			break;
		}
	}

	return surfaceFormat;
}

VkCompositeAlphaFlagBitsKHR EngineSwapChain::findCompositeAlpha(const VkSurfaceCapabilitiesKHR& a_surfCaps)
{
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	static constexpr std::array<VkCompositeAlphaFlagBitsKHR, 4>  compositeAlphaFlags = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};

	for (const auto& compositeAlphaFlag : compositeAlphaFlags) {
		if (a_surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
			compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	return compositeAlpha;
}

VkExtent2D EngineSwapChain::getImageExtent(const VkSurfaceCapabilitiesKHR& a_surfCaps, const uint32_t a_width, const uint32_t a_height)
{
	VkExtent2D extent = a_surfCaps.currentExtent;
	if (a_surfCaps.currentExtent.width == (uint32_t)-1)
	{
		// If the surface size is undefined, the size is set to
		// the size of the images requested.
		extent.width = a_width;
		extent.height = a_height;
	}
	return extent;
}

void EngineSwapChain::createImageBuffers()
{
	std::vector<VkImage> images;
	VK_CHECK_EXCEPT(enumerateEx(&vkGetSwapchainImagesKHR, images, VK_SUCCESS, m_deviceCtx.m_vkDevice, m_swapChain))

	for (const VkImage img : images)
	{
		VkImageView imageView = VK_NULL_HANDLE;

		const ImageViewParameters parameters
		{
			.colorFormat = m_swapchainCI.imageFormat,
			.image = img,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.flag = 0,
			.subResource = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		VkImageViewCreateInfo colorAttachmentView = imageViewCreateInfo(parameters);


		VK_CHECK_EXCEPT(vkCreateImageView(m_deviceCtx.m_vkDevice, &colorAttachmentView, nullptr, &imageView))

		m_frames.emplace_back(SwapChainFrame{
				img,
				imageView
			});
	}
}


void EngineSwapChain::releaseSwapchain(VkSwapchainKHR a_oldSwapChain)
{
	for (const auto& img : m_frames)
		vkDestroyImageView(m_deviceCtx.m_vkDevice, img.m_imageView, nullptr);
	vkDestroySwapchainKHR(m_deviceCtx.m_vkDevice, a_oldSwapChain, nullptr);
	m_frames.clear();
}

void EngineSwapChain::createSwapChain(const uint32_t a_width, const uint32_t a_height)
{
	if (m_frames.empty())
	{
		// new swapchain
		VK_CHECK_EXCEPT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_deviceCtx.m_vkPhysDevice, m_surface, &m_surfCaps))

		std::vector<VkPresentModeKHR> presentationModes;
		enumerate(&vkGetPhysicalDeviceSurfacePresentModesKHR, presentationModes, m_deviceCtx.m_vkPhysDevice, m_surface);

		// Determine the number of images in swapchain
		const uint32_t desiredNumberOfSwapchainImages = EngineSwapChain::getImageCount(m_surfCaps);

		// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
		// This mode waits for the vertical blank ("v-sync")
		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		VkSurfaceFormatKHR imageFormat = EngineSwapChain::findSurfaceFormat(m_deviceCtx.m_vkPhysDevice, m_surface);

		VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | m_surfCaps.supportedUsageFlags;

		const VkExtent2D swapchainExtent = EngineSwapChain::getImageExtent(m_surfCaps, a_width, a_height);

		// create info with the current swapchain
		m_swapchainCI = swapChainCreateInfoKHR(m_surface, imageFormat,
			desiredNumberOfSwapchainImages, swapchainExtent, usage, m_surfCaps.currentTransform,
			EngineSwapChain::findCompositeAlpha(m_surfCaps), swapchainPresentMode, VK_TRUE, m_swapChain);

		VK_CHECK_EXCEPT(vkCreateSwapchainKHR(m_deviceCtx.m_vkDevice, &m_swapchainCI, nullptr, &m_swapChain))
	}
	else
	{
		auto oldSwapChain = m_swapChain;
		m_swapchainCI.imageExtent = EngineSwapChain::getImageExtent(m_surfCaps, a_width, a_height);
		m_swapchainCI.oldSwapchain = oldSwapChain;
		VK_CHECK_EXCEPT(vkCreateSwapchainKHR(m_deviceCtx.m_vkDevice, &m_swapchainCI, nullptr, &m_swapChain))
		releaseSwapchain(oldSwapChain);
	}
	createImageBuffers();
}