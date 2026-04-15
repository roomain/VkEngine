#pragma once
/***********************************************
* @headerfile CheckParameters.h
* @date 20 / 02 / 2026
* @author Roomain
************************************************/
#include <vector>

struct DeviceParameters;
struct VulkanCapabilities;
struct DeviceFeatures;
struct VkPhysicalDeviceFeatures;

/*@brief functions used to check vulkan devices compatibilities*/
template<typename T, typename U, typename V = T>
constexpr bool contains(const std::vector<T>& a_desired, const std::vector<U>& a_available,
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

constexpr bool check(const bool a_desired, const bool a_available)
{
    return !a_desired || a_available;
}

bool checkFeatures(const DeviceFeatures& a_desired, const VkPhysicalDeviceFeatures& a_available);
std::vector<int> findSuitableDevice(const DeviceParameters& a_dev, const VulkanCapabilities& a_capabilities);