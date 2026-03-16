#pragma once
/***********************************************
* @headerfile EngineApplication.h
* @date 13 / 03 / 2026
* @author Roomain
************************************************/
#include <string>
#include <vector>
#include <memory>
#include <vulkan/vulkan.hpp>
#include "VulkanCapabilities.h"

class EngineDevice;
using EngineDevicePtr = std::shared_ptr<EngineDevice>;

/*@brief entry point of engine*/
class EngineApplication
{
private:
	static constexpr uint32_t ENGINE_VERSION = 1;
	static inline const std::string ENGINE_NAME = "VkEngine";

	// contains application instance and physical devices
	VulkanCapabilities m_capabilities;				/*!< vulkan engine capabilities*/
	std::vector<EngineDevicePtr> m_deviceInstance;	/*!< instanciated devices*/

public:
	EngineApplication() = delete;
	explicit EngineApplication(const std::string& a_appName, const uint32_t a_appVersion);
	// todo

#pragma region devices
	// todo
#pragma endregion //devices
};

