#pragma once
/***********************************************
* @headerfile CapabilitiesVisitor.h
* @date 22 / 02 / 2026
* @author Roomain
************************************************/

#include <cstdint>
#include <string_view>
#include <string>
#include <vector>
#include <format>
#include "enginecore_globals.h"


struct BaseVulkanCapabilities;
struct VulkanDeviceCapabilities;
struct VulkanCapabilities;

struct VkExtensionProperties;
struct VkLayerProperties;
struct VkPhysicalDeviceProperties;
struct VkPhysicalDeviceLimits;
struct VkPhysicalDeviceSparseProperties;
struct VkPhysicalDeviceFeatures;

/*@brief base class to display vulkan capabilities*/
class ENGINECORE_EXPORT CapabilitiesVisitor
{
	friend class Engine;
private:

	template<typename SizeType>
	static void memorySize(const SizeType& a_size, double& a_computedSize, std::string& a_unit)
	{
		static constexpr double kilo = 1024;
		static constexpr double mega = 1024 * kilo;
		static constexpr double giga = 1024 * mega;

		if (a_computedSize = static_cast<double>(a_size) / giga; a_computedSize > 0.01)
		{
			a_unit = "Go";
		}
		else if (a_computedSize = static_cast<double>(a_size) / giga; a_computedSize > 0.01)
		{
			a_unit = "Mo";
		}
		else if (a_computedSize = static_cast<double>(a_size) / kilo; a_computedSize > 0.01)
		{
			a_unit = "Ko";
		}
		else
		{
			a_computedSize = static_cast<double>(a_size);
			a_unit = "o";
		}
	}

	template<typename SizeType>
	static std::string memorySize(const SizeType& a_size)
	{
		double formalizedSize;
		std::string unit;
		CapabilitiesVisitor::memorySize(a_size, formalizedSize, unit);
		return std::format("{} {}", formalizedSize, unit).c_str();
	}

	void visit(const VulkanCapabilities& a_cap);
	void visit(const VulkanDeviceCapabilities& a_cap);
	void visit(const std::vector<VkExtensionProperties>& a_ext);
	void visit(const std::vector<VkLayerProperties>& a_layers);
	void visit(const VkPhysicalDeviceProperties& a_prop);
	void visit(const VkPhysicalDeviceLimits& a_limits);
	void visit(const VkPhysicalDeviceSparseProperties& a_sparseProp);
	void visit(const VkPhysicalDeviceFeatures& a_features);
	static std::string versionToString(const uint32_t a_version);

public:
	virtual ~CapabilitiesVisitor() = default;

protected:
	/*@brief to display data in a tree structure*/
	virtual void beginNode(const char* a_title) = 0;
	virtual void endNode() = 0;

	/*@brief to display multiple data for single capavity*/
	virtual void benginDataArray(const char* a_title) = 0;
	virtual void endDataArray() = 0;

	virtual void visitData(const char* a_title, const bool a_value) = 0;
	virtual void visitData(const char* a_title, const int32_t a_value) = 0;
	virtual void visitData(const char* a_title, const uint32_t a_value) = 0;
	virtual void visitData(const char* a_title, const float a_value) = 0;
	virtual void visitData(const char* a_title, const double& a_value) = 0;
	virtual void visitData(const char* a_title, const char* a_value) = 0;
	virtual void visitData(const char* a_title, const std::string_view& a_value) = 0;
	virtual void visitData(const char* a_title, const std::vector<std::string>& a_value) = 0;
};