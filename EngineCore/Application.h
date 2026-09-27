#pragma once
/***********************************************
* @headerfile Application.h
* @date 13 / 03 / 2026
* @author Roomain
************************************************/
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <vulkan/vulkan.hpp>
#include "VulkanCapabilities.h"
#include "InternalConfiguration.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

class Device;
using DevicePtr = std::shared_ptr<Device>;

class Renderer;
using RendererPtr = std::shared_ptr<Renderer>;

struct DeviceParameters;
struct RendererParameters;

/*@brief Application mandatory parameters */
struct ApplicationParameters
{
	std::string appName;				/*!< application name*/
	std::string parametersFilename;		/*!< parameters file contains witch contains vulkan parameters by profile*/
	std::string parametersProfile;		/*!< parameters profile to use*/
	uint32_t appVersion;				/*!< application version*/
};

/*@brief entry point of engine*/
class ENGINECORE_EXPORT Application
{
private:
	static constexpr uint32_t ENGINE_VERSION = 1;
	static inline const std::string ENGINE_NAME = "VkEngine";

	// contains application instance and physical devices
	VulkanCapabilities m_capabilities;							/*!< vulkan engine capabilities*/
	std::vector<DevicePtr> m_deviceInstance;				/*!< instanciated devices*/
	VkDebugUtilsMessengerEXT  m_debugMessenger = VK_NULL_HANDLE;/*!< vulkan debug handle*/

public:
	Application() = delete;
	explicit Application(const ApplicationParameters& a_appParameters);
	~Application();
	[[nodiscard]] inline const VulkanCapabilities& capabilities()const noexcept { return m_capabilities; }
#pragma region devices
	[[nodiscard]] std::vector<DeviceConfiguration> suitableDevices(const DeviceParameters& a_parameters, const VkSurfaceKHR* a_surface = VK_NULL_HANDLE)const;
	[[nodiscard]] DevicePtr createDevice(const DeviceConfiguration& a_parameters, bool a_enableDynRendering = false);
	[[nodiscard]] RendererPtr createRenderer(const RendererConfiguration& a_parameter);
	// todo save device
#pragma endregion //devices
	static VulkanCapabilities hostCapabilities();
};

#pragma warning(pop)
