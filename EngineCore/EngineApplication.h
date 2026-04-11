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

class EngineRenderer;
using EngineRendererPtr = std::shared_ptr<EngineRenderer>;

struct DeviceParameters;
struct RendererParameters;


struct EngineApplicationParameters
{
	std::string appName;				/*!< application name*/
	std::string parametersFilename;		/*!< parameters file*/
	std::string parametersProfile;		/*!< parameters profile*/
	uint32_t appVersion;				/*!< application version*/
};

/*@brief entry point of engine*/
class EngineApplication
{
private:
	static constexpr uint32_t ENGINE_VERSION = 1;
	static inline const std::string ENGINE_NAME = "VkEngine";

	// contains application instance and physical devices
	VulkanCapabilities m_capabilities;							/*!< vulkan engine capabilities*/
	std::vector<EngineDevicePtr> m_deviceInstance;				/*!< instanciated devices*/
	VkDebugUtilsMessengerEXT  m_debugMessenger = VK_NULL_HANDLE;/*!< vulkan debug handle*/

public:
	EngineApplication() = delete;
	explicit EngineApplication(const EngineApplicationParameters& a_appParameters);
	~EngineApplication();

#pragma region devices
	EngineDevicePtr createDevice(const DeviceParameters& a_parameters);
	EngineRendererPtr createRenderer(const RendererParameters& a_parameters);
	// todo
#pragma endregion //devices
};

