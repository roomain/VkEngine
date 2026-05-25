#include "CapabilitiesDatabase.h"
#include "EngineApplication.h"
#include "EngineParameters.h"


CapabilitiesDatabase::CapabilitiesDatabase()
{
	m_capabilities = EngineApplication::hostCapabilities();
}

CapabilitiesDatabase& CapabilitiesDatabase::instance()
{
	static CapabilitiesDatabase s_instance;
	return s_instance;
}


std::vector<std::string> CapabilitiesDatabase::instanceLayers()const
{
	std::vector<std::string> layers;
	for (const auto& layer : m_capabilities.layers)
		layers.push_back(layer.layerName);
	return layers;
}

std::vector<std::string> CapabilitiesDatabase::instanceExtensions()const
{
	std::vector<std::string> extensions;
	for (const auto& ext : m_capabilities.extensions)
		extensions.push_back(ext.extensionName);
	return extensions;
}

std::vector<std::string> CapabilitiesDatabase::deviceLayers()const
{
	std::vector<std::string> layers;
	for (const auto& dev : m_capabilities.devices)
	{
		for (const auto& layer : dev.layers)
			layers.push_back(layer.layerName);
	}
	return removeDoubloon(layers);
}

std::vector<std::string> CapabilitiesDatabase::deviceExtensions()const
{
	std::vector<std::string> extensions;
	for (const auto& dev : m_capabilities.devices)
	{
		for (const auto& ext : dev.extensions)
			extensions.push_back(ext.extensionName);
	}
	return removeDoubloon(extensions);
}