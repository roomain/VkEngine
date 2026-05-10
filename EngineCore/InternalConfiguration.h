#pragma once
/***********************************************
* @headerfile InternalConfiguration.h
* @date 10 / 05 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <vulkan/vulkan.hpp>

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

/*@brief surface configuration*/
struct SurfaceConfiguration
{
    VkSurfaceKHR surface;   /*!< vulkan surface*/
    uint32_t width;         /*!< surface width*/
    uint32_t height;        /*!< surface height*/
};

/*@brief Rendering configuration*/
struct RendererConfiguration
{
    DeviceConfiguration deviceConf;     /*!< device configuraton*/
    SurfaceConfiguration surfaceConf;   /*!< surface configuration*/
};