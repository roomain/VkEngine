#include "pch.h"
#include "CheckParameters.h"
#include "VulkanCapabilities.h"
#include "EngineParameters.h"

bool checkFeatures(const DeviceFeatures& a_desired, const VkPhysicalDeviceFeatures& a_available)
{
	bool bCheck = check(a_desired.robustBufferAccess, a_available.robustBufferAccess);
	bCheck &= check(a_desired.fullDrawIndexUint32, a_available.fullDrawIndexUint32);
	bCheck &= check(a_desired.imageCubeArray, a_available.imageCubeArray);
	bCheck &= check(a_desired.independentBlend, a_available.independentBlend);
	bCheck &= check(a_desired.geometryShader, a_available.geometryShader);
	bCheck &= check(a_desired.tessellationShader, a_available.tessellationShader);
	bCheck &= check(a_desired.sampleRateShading, a_available.sampleRateShading);
	bCheck &= check(a_desired.dualSrcBlend, a_available.dualSrcBlend);
	bCheck &= check(a_desired.logicOp, a_available.logicOp);
	bCheck &= check(a_desired.multiDrawIndirect, a_available.multiDrawIndirect);
	bCheck &= check(a_desired.drawIndirectFirstInstance, a_available.drawIndirectFirstInstance);
	bCheck &= check(a_desired.depthClamp, a_available.depthClamp);
	bCheck &= check(a_desired.depthBiasClamp, a_available.depthBiasClamp);
	bCheck &= check(a_desired.fillModeNonSolid, a_available.fillModeNonSolid);
	bCheck &= check(a_desired.depthBounds, a_available.depthBounds);
	bCheck &= check(a_desired.wideLines, a_available.wideLines);
	bCheck &= check(a_desired.largePoints, a_available.largePoints);
	bCheck &= check(a_desired.alphaToOne, a_available.alphaToOne);
	bCheck &= check(a_desired.multiViewport, a_available.multiViewport);
	bCheck &= check(a_desired.samplerAnisotropy, a_available.samplerAnisotropy);
	bCheck &= check(a_desired.textureCompressionETC2, a_available.textureCompressionETC2);
	bCheck &= check(a_desired.textureCompressionASTC_LDR, a_available.textureCompressionASTC_LDR);
	bCheck &= check(a_desired.textureCompressionBC, a_available.textureCompressionBC);
	bCheck &= check(a_desired.occlusionQueryPrecise, a_available.occlusionQueryPrecise);
	bCheck &= check(a_desired.pipelineStatisticsQuery, a_available.pipelineStatisticsQuery);
	bCheck &= check(a_desired.vertexPipelineStoresAndAtomics, a_available.vertexPipelineStoresAndAtomics);
	bCheck &= check(a_desired.fragmentStoresAndAtomics, a_available.fragmentStoresAndAtomics);
	bCheck &= check(a_desired.shaderTessellationAndGeometryPointSize, a_available.shaderTessellationAndGeometryPointSize);
	bCheck &= check(a_desired.shaderImageGatherExtended, a_available.shaderImageGatherExtended);
	bCheck &= check(a_desired.shaderStorageImageExtendedFormats, a_available.shaderStorageImageExtendedFormats);
	bCheck &= check(a_desired.shaderStorageImageMultisample, a_available.shaderStorageImageMultisample);
	bCheck &= check(a_desired.shaderStorageImageReadWithoutFormat, a_available.shaderStorageImageReadWithoutFormat);
	bCheck &= check(a_desired.shaderStorageImageWriteWithoutFormat, a_available.shaderStorageImageWriteWithoutFormat);
	bCheck &= check(a_desired.shaderUniformBufferArrayDynamicIndexing, a_available.shaderUniformBufferArrayDynamicIndexing);
	bCheck &= check(a_desired.shaderSampledImageArrayDynamicIndexing, a_available.shaderSampledImageArrayDynamicIndexing);
	bCheck &= check(a_desired.shaderStorageBufferArrayDynamicIndexing, a_available.shaderStorageBufferArrayDynamicIndexing);
	bCheck &= check(a_desired.shaderStorageImageArrayDynamicIndexing, a_available.shaderStorageImageArrayDynamicIndexing);
	bCheck &= check(a_desired.shaderClipDistance, a_available.shaderClipDistance);
	bCheck &= check(a_desired.shaderCullDistance, a_available.shaderCullDistance);
	bCheck &= check(a_desired.shaderFloat64, a_available.shaderFloat64);
	bCheck &= check(a_desired.shaderInt64, a_available.shaderInt64);
	bCheck &= check(a_desired.shaderInt16, a_available.shaderInt16);
	bCheck &= check(a_desired.shaderResourceResidency, a_available.shaderResourceResidency);
	bCheck &= check(a_desired.shaderResourceMinLod, a_available.shaderResourceMinLod);
	bCheck &= check(a_desired.sparseBinding, a_available.sparseBinding);
	bCheck &= check(a_desired.sparseResidencyBuffer, a_available.sparseResidencyBuffer);
	bCheck &= check(a_desired.sparseResidencyImage2D, a_available.sparseResidencyImage2D);
	bCheck &= check(a_desired.sparseResidencyImage3D, a_available.sparseResidencyImage3D);
	bCheck &= check(a_desired.sparseResidency2Samples, a_available.sparseResidency2Samples);
	bCheck &= check(a_desired.sparseResidency4Samples, a_available.sparseResidency4Samples);
	bCheck &= check(a_desired.sparseResidency8Samples, a_available.sparseResidency8Samples);
	bCheck &= check(a_desired.sparseResidency16Samples, a_available.sparseResidency16Samples);
	bCheck &= check(a_desired.sparseResidencyAliased, a_available.sparseResidencyAliased);
	bCheck &= check(a_desired.variableMultisampleRate, a_available.variableMultisampleRate);
	bCheck &= check(a_desired.inheritedQueries, a_available.inheritedQueries);
	return bCheck;
}

std::vector<int> findSuitableDevice(const DeviceParameters& a_dev, const VulkanCapabilities& a_capabilities)
{
	std::vector<int> deviceIndices;
	int a_deviceIndex = 0;
	for (const auto& device : a_capabilities.devices)
	{
		if (contains(a_dev.extensions, device.extensions, &VkExtensionProperties::extensionName)
			&& contains(a_dev.layers, device.layers, &VkLayerProperties::layerName)
			&& checkFeatures(a_dev.features, device.features))
		{
			deviceIndices.emplace_back(a_deviceIndex);
		}
		++a_deviceIndex;
	}
	return deviceIndices;
}
