#pragma once
#include <vector>
#include <string>
#include "VulkanCapabilities.h"

class CapabilitiesDatabase
{
private:
	CapabilitiesDatabase();
	VulkanCapabilities m_capabilities;

public:
	static CapabilitiesDatabase& instance();
	const VulkanCapabilities& capabilities()const { return m_capabilities; }
	std::vector<std::string> instanceLayers()const;
	std::vector<std::string> instanceExtensions()const;
	std::vector<std::string> deviceLayers()const;
	std::vector<std::string> deviceExtensions()const;
};

