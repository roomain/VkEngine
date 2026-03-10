#pragma once
/***********************************************
* @headerfile VulkanInitializers.h
* @date 02 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <string>
#include <vulkan/vulkan.h>

[[nodiscard]] constexpr VkDebugReportCallbackCreateInfoEXT initDebugCallbackCreateInfo()
{
	return VkDebugReportCallbackCreateInfoEXT{ .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT };
}

[[nodiscard]] constexpr VkDebugUtilsMessengerCreateInfoEXT initMessageCallbackCreateInfo()
{
	return VkDebugUtilsMessengerCreateInfoEXT{ .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = nullptr ,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pUserData = nullptr
	};
}

[[nodiscard]] inline VkApplicationInfo initApplicationInfo(
	const std::string& a_appName, const std::string& a_engineName, 
	const uint32_t a_appVersion, const uint32_t a_engineVersion)
{
	uint32_t apiVersion = VK_API_VERSION_1_0;
	vkEnumerateInstanceVersion(&apiVersion);
	return VkApplicationInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = a_appName.c_str(),
		.applicationVersion = a_appVersion,
		.pEngineName = a_engineName.c_str(),
		.engineVersion = a_engineVersion,
		.apiVersion = apiVersion
	};
}

[[nodiscard]] constexpr VkInstanceCreateInfo initInstanceCreateInfo(const VkApplicationInfo* a_appInfo)
{
	return VkInstanceCreateInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.pApplicationInfo = a_appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = nullptr
	};
}

template<typename QueueInfoContainer>
[[nodiscard]] inline VkDeviceCreateInfo initDeviceCreateInfo(const QueueInfoContainer& a_queueInfo,
	const VkPhysicalDeviceFeatures* a_features, const VkDeviceCreateFlags a_flags)
{
	return VkDeviceCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.queueCreateInfoCount = static_cast<uint32_t>(a_queueInfo.size()),
		.pQueueCreateInfos = a_queueInfo.data(),
		.pEnabledFeatures = a_features
	};
}

template<typename PrioritiesContainer>
[[nodiscard]] constexpr VkDeviceQueueCreateInfo initQueueCreateInfo(const uint32_t a_familyIndex,
	const uint32_t a_queueCount,
	const PrioritiesContainer& a_priorities,
	const VkDeviceQueueCreateFlags a_flag = 0)
{
	return VkDeviceQueueCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flag,
		.queueFamilyIndex = a_familyIndex,
		.queueCount = a_queueCount,
		.pQueuePriorities = a_priorities.data()
	};
}

template<typename ImageViewContainer>
[[nodiscard]] constexpr VkFramebufferCreateInfo initFrameBufferCreateInfo(const VkRenderPass a_renderPass,
	const ImageViewContainer& a_attachment, const uint32_t a_Width, const uint32_t a_Height)
{
	return VkFramebufferCreateInfo{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext = nullptr,
		.renderPass = a_renderPass,
		.attachmentCount = static_cast<uint32_t>(a_attachment.size()),
		.pAttachments = a_attachment.data(),
		.width = a_Width,
		.height = a_Height,
		.layers = 1
	};
}

[[nodiscard]] constexpr VkSwapchainCreateInfoKHR swapChainCreateInfoKHR(const VkSurfaceKHR a_surface,
	const VkSurfaceFormatKHR& a_format,
	const uint32_t a_imageCount,
	const VkExtent2D& a_extent,
	const VkImageUsageFlags a_usage,
	const VkSurfaceTransformFlagBitsKHR a_transform,
	const VkCompositeAlphaFlagBitsKHR a_compositeFlag,
	const VkPresentModeKHR a_presentMode,
	const VkBool32 a_clipped,
	VkSwapchainKHR a_oldSwapChain = VK_NULL_HANDLE
)
{
	return VkSwapchainCreateInfoKHR{ .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = 0,
		.surface = a_surface,
		.minImageCount = a_imageCount,
		.imageFormat = a_format.format,
		.imageColorSpace = a_format.colorSpace,
		.imageExtent = a_extent,
		.imageArrayLayers = 1,
		.imageUsage = a_usage,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,	// no sharing
		.queueFamilyIndexCount = 0,						// number of shared queues
		.pQueueFamilyIndices = nullptr,					// shared queues
		.preTransform = a_transform,
		.compositeAlpha = a_compositeFlag,
		.presentMode = a_presentMode,
		.clipped = a_clipped,
		.oldSwapchain = a_oldSwapChain
	};
}

[[nodiscard]] constexpr VkPresentInfoKHR presentationKHR(const uint32_t& a_semaphoreCount, VkSemaphore* a_pSemaphore, const uint32_t& a_swapChainCount, const VkSwapchainKHR* a_pSwapChain, const uint32_t* a_pImgIndices)
{
	return VkPresentInfoKHR{
	.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	.pNext = nullptr,
	.waitSemaphoreCount = a_semaphoreCount,
	.pWaitSemaphores = a_pSemaphore,
	.swapchainCount = a_swapChainCount,
	.pSwapchains = a_pSwapChain,
	.pImageIndices = a_pImgIndices,
	.pResults = nullptr
	};
}

[[nodiscard]] constexpr VkSemaphoreCreateInfo initSemaphoreCreateInfo(VkSemaphoreCreateFlags a_flags = 0)
{
	return VkSemaphoreCreateInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = nullptr, .flags = a_flags };
}

[[nodiscard]] constexpr VkFenceCreateInfo initFenceCreateInfo(VkFenceCreateFlags a_flags = 0)
{
	return VkFenceCreateInfo{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = nullptr, .flags = a_flags };
}

struct FrameBufferParameters
{
	VkRenderPass renderPass;
	std::vector<VkImageView> attachments;
	uint32_t width;
	uint32_t height;
	uint32_t layers = 1;
	VkFramebufferCreateFlags flags = 0;
};

[[nodiscard]] constexpr VkFramebufferCreateInfo initFrameBufferCreateInfo(const FrameBufferParameters& params)
{
	return VkFramebufferCreateInfo{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext = nullptr,
		.flags = params.flags,
		.attachmentCount = static_cast<uint32_t>(params.attachments.size()),
		.pAttachments = params.attachments.data(),
		.width = params.width,
		.height = params.height,
		.layers = params.layers
	};
}

[[nodiscard]] constexpr VkRayTracingShaderGroupCreateInfoKHR initRayTracingShaderGroupCreateInfoKHR()
{
	return VkRayTracingShaderGroupCreateInfoKHR{
	.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
	.pNext = nullptr };
}

[[nodiscard]] constexpr VkRayTracingPipelineCreateInfoKHR initRrayTracingPipelineCreateInfoKHR()
{
	return VkRayTracingPipelineCreateInfoKHR{
	.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
	.pNext = nullptr };
}