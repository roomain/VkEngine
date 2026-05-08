#pragma once
/***********************************************
* @headerfile CheckParameters.h
* @date 20 / 02 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <vulkan/vulkan.hpp>

struct DeviceParameters;
struct VulkanCapabilities;
struct DeviceFeatures;
struct VkPhysicalDeviceFeatures;

/*@brief functions used to check vulkan devices compatibilities*/
template<typename T, typename U, typename V = T>
[[nodiscard]] constexpr bool contains(const std::vector<T>& a_desired, const std::vector<U>& a_available,
    V U::* a_internPtr)
{
    for (const auto& desired : a_desired)
    {
        if (std::ranges::find_if(a_available, [&desired, a_internPtr](const auto& a_dat)
            {
                return desired == T(a_dat.*(a_internPtr));
            }) == a_available.cend())
        {
            return false;
        }
    }
    return true;
}

[[nodiscard]] constexpr bool check(const bool a_desired, const bool a_available)
{
    return !a_desired || a_available;
}

//[[nodiscard]] bool checkQueue(const VkQueueFamilyProperties& a_family, const VkQueueFlags a_expectedFlag = 0, const uint32t)

[[nodiscard]] bool checkFeatures(const DeviceFeatures& a_desired, const VkPhysicalDeviceFeatures& a_available);


/*@brief queue configuration corresponding to queue parameters*/
struct QueueConfiguration
{
    uint32_t familyIndex;   /*!< queue family index*/
    VkQueueFlags flags;     /*!< queue family flags*/
    uint32_t queueCount;    /*!< queue count*/
    float priority;         /*!< queue priority*/
};

/*@brief device configuration corresponding to device parameters*/
struct DeviceConfiguration
{
    uint32_t deviceIndex;                   /*!< device index*/
    std::vector<std::string> extensions;    /*!< device layers*/
    std::vector<std::string> layers;        /*!< device extension*/
    VkPhysicalDeviceFeatures features;      /*!< device features*/
    std::vector<QueueConfiguration> queues; /*!< queues configuration*/
};

[[nodiscard]] std::vector<DeviceConfiguration> findSuitableDevices(const DeviceParameters& a_devParameters, const VulkanCapabilities& a_capabilities, const VkSurfaceKHR* a_surface);