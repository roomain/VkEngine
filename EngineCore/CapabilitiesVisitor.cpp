#include "pch.h"

#include "CapabilitiesVisitor.h"

#include <vulkan/vulkan.hpp>

std::string vendorToString(uint32_t vendorID)
{
	switch (vendorID)
	{
	case 0x10DE: 
		return "NVIDIA";
	case 0x1002: 
		return "AMD";
	case 0x8086: 
		return "Intel";
	case 0x13B5: 
		return "ARM";
	case 0x5143: 
		return "Qualcomm";
	case 0x1010: 
		return "Imagination Technologies";
	case 0x106B: 
		return "Apple";
	default:     
		return "Unknown vendor";
	}
}

std::string CapabilitiesVisitor::versionToString(const uint32_t a_version)
{
	return  std::to_string(a_version >> 22) + "." + std::to_string((a_version >> 12) & 0x3ff) + "." + std::to_string(a_version & 0xfff);
}

void CapabilitiesVisitor::visit(const std::vector<VkExtensionProperties>& a_ext)
{
	beginNode("Extensions");
	std::vector<std::string> extensions;
	for (const auto& extension : a_ext)
		extensions.emplace_back(std::format("{} v{}", extension.extensionName, CapabilitiesVisitor::versionToString(extension.specVersion)));

	visitData("Extensions", extensions);
	endNode();
}

void CapabilitiesVisitor::visit(const std::vector<VkLayerProperties>& a_layers)
{
	beginNode("Layers");
	for (const auto& layer : a_layers)
	{
		benginDataArray(layer.layerName);
		visitData("Description", layer.description);
		visitData("Version", CapabilitiesVisitor::versionToString(layer.specVersion));	
		visitData("Implementation version", CapabilitiesVisitor::versionToString(layer.implementationVersion));
		endDataArray();
	}
	endNode();
}

void CapabilitiesVisitor::visit(const VulkanCapabilities& a_cap)
{
	beginNode("Vulkan capabilities");
	uint32_t apiVersion = VK_API_VERSION_1_0;
	vkEnumerateInstanceVersion(&apiVersion);
	visitData("API version", CapabilitiesVisitor::versionToString(apiVersion));
	visit(a_cap.extensions);
	visit(a_cap.layers);

	beginNode("Devices");
	for (const auto& device : a_cap.devices)
		visit(device);
	endNode();
	
	endNode();
}

void CapabilitiesVisitor::visit(const VulkanDeviceCapabilities& a_cap)
{
	beginNode(a_cap.properties.deviceName);
	
	benginDataArray("Properties");
	visit(a_cap.properties);
	endDataArray();

    benginDataArray("Properties");
    visit(a_cap.features);
    endDataArray();

    benginDataArray("Memory");
    for (uint32_t index = 0; index < a_cap.memoryProperties.memoryTypeCount; ++index)
    {
        visitData("Mem type flags", Flag<VkMemoryPropertyFlagBits>::to_string(a_cap.memoryProperties.memoryTypes[index].propertyFlags).c_str());
        visitData("Mem type Heap index", std::to_string(a_cap.memoryProperties.memoryTypes[index].heapIndex).c_str());
    }

    for (uint32_t index = 0; index < a_cap.memoryProperties.memoryHeapCount; ++index)
    {
        visitData("Mem heap flags", Flag<VkMemoryHeapFlagBits>::to_string(a_cap.memoryProperties.memoryHeaps[index].flags).c_str());
        visitData("Mem heap size", CapabilitiesVisitor::memorySize(a_cap.memoryProperties.memoryHeaps[index].size).c_str());
    }
    endDataArray();

    beginNode("Queue family");
    int index = 0;
	for (const auto& queueFamily : a_cap.queueFamilies)
    {
        beginNode(std::format("Queue family {}", index).c_str());
		benginDataArray("Properties");
        visitData("Queue family flags", Flag<VkQueueFlagBits>::to_string(queueFamily.queueFlags));
        visitData("Queue count", queueFamily.queueCount);
        visitData("Timestamp valid bits", queueFamily.timestampValidBits);
        visitData("Min image transfer granularity [0]", queueFamily.minImageTransferGranularity.width);
        visitData("Min image transfer granularity [1]", queueFamily.minImageTransferGranularity.height);
        visitData("Min image transfer granularity [2]", queueFamily.minImageTransferGranularity.depth);
        endDataArray();
        endNode();
        ++index;
    }
    endNode();

	endNode();
}

void CapabilitiesVisitor::visit(const VkPhysicalDeviceProperties& a_prop)
{
	visitData("API version", CapabilitiesVisitor::versionToString(a_prop.apiVersion));
	visitData("Driver version", CapabilitiesVisitor::versionToString(a_prop.driverVersion));
	visitData("Vendor", vendorToString(a_prop.vendorID));
	visitData("Device type", to_string(a_prop.deviceType));

	benginDataArray("Limits");
	visit(a_prop.limits);
	endDataArray();

    benginDataArray("Sparse properties");
    visit(a_prop.sparseProperties);
    endDataArray();

}

void CapabilitiesVisitor::visit(const VkPhysicalDeviceLimits& a_limits)
{
    visitData("Max image dim 1D", a_limits.maxImageDimension1D);
    visitData("Max image dim 2D", a_limits.maxImageDimension2D);
    visitData("Max image dim 3D", a_limits.maxImageDimension3D);
    visitData("Max image dim cube", a_limits.maxImageDimensionCube);
    visitData("Max image array layer", a_limits.maxImageArrayLayers);
    visitData("Max texel buffer element", a_limits.maxTexelBufferElements);
    visitData("Max uniform buffer range", a_limits.maxUniformBufferRange);
    visitData("Max storage buffer range", a_limits.maxStorageBufferRange);
    visitData("Max push constant size", a_limits.maxPushConstantsSize);
    visitData("Max memory allocation count", a_limits.maxMemoryAllocationCount);
    visitData("Max sampler allocation count", a_limits.maxSamplerAllocationCount);
    visitData("Buffer image granularity", CapabilitiesVisitor::memorySize(a_limits.bufferImageGranularity).c_str());
    visitData("Sparse address size", CapabilitiesVisitor::memorySize(a_limits.sparseAddressSpaceSize).c_str());
    visitData("Max bound descriptor sets", a_limits.maxBoundDescriptorSets);
    visitData("Max per stage descriptor samplers", a_limits.maxPerStageDescriptorSamplers);
    visitData("Max per stage descriptor uniform buffers", a_limits.maxPerStageDescriptorUniformBuffers);
    visitData("Max per stage descriptor storage buffers", a_limits.maxPerStageDescriptorStorageBuffers);
    visitData("Max per stage descriptor sampled images", a_limits.maxPerStageDescriptorSampledImages);
    visitData("Max per stage descriptor storage images", a_limits.maxPerStageDescriptorStorageImages);
    visitData("Max per stage descriptor input attachment", a_limits.maxPerStageDescriptorInputAttachments);
    visitData("Max per stage resources", a_limits.maxPerStageResources);
    visitData("Max descriptor set samplers ", a_limits.maxDescriptorSetSamplers);
    visitData("Max descriptor set uniform buffers", a_limits.maxDescriptorSetUniformBuffers);
    visitData("Max descriptor set uniform buffers dynamic", a_limits.maxDescriptorSetUniformBuffersDynamic);
    visitData("Max descriptor set storage buffers", a_limits.maxDescriptorSetStorageBuffers);
    visitData("Max descriptor set storage buffers dynamic", a_limits.maxDescriptorSetStorageBuffersDynamic);
    visitData("Max descriptor set sampled images", a_limits.maxDescriptorSetSampledImages);
    visitData("Max descriptor set storage images", a_limits.maxDescriptorSetStorageImages);
    visitData("Max descriptor set input attachment", a_limits.maxDescriptorSetInputAttachments);
    visitData("Max vertex input attributes", a_limits.maxVertexInputAttributes);
    visitData("Max vertex input bindings", a_limits.maxVertexInputBindings);
    visitData("Max vertex input attibute offset", a_limits.maxVertexInputAttributeOffset);
    visitData("Max vertex input binding stride", a_limits.maxVertexInputBindingStride);
    visitData("Max vertex output components", a_limits.maxVertexOutputComponents);
    visitData("Max tesselation generation level", a_limits.maxTessellationGenerationLevel);
    visitData("Max tesselation patch size", a_limits.maxTessellationPatchSize);
    visitData("Max tesselation control per vertex input components", a_limits.maxTessellationControlPerVertexInputComponents);
    visitData("Max tesselation control per vertex output components", a_limits.maxTessellationControlPerVertexOutputComponents);
    visitData("Max tesselation control per patch output components", a_limits.maxTessellationControlPerPatchOutputComponents);
    visitData("Max tesselation control total output components", a_limits.maxTessellationControlTotalOutputComponents);
    visitData("Max tesselation evaluation input components", a_limits.maxTessellationEvaluationInputComponents);
    visitData("Max tesselation evaluation output components", a_limits.maxTessellationEvaluationOutputComponents);
    visitData("Max geometry shader invocations", a_limits.maxGeometryShaderInvocations);
    visitData("Max geometry input components", a_limits.maxGeometryInputComponents);
    visitData("Max geometry output components", a_limits.maxGeometryOutputComponents);
    visitData("Max geometry output vertices", a_limits.maxGeometryOutputVertices);
    visitData("Max geometry total output components", a_limits.maxGeometryTotalOutputComponents);
    visitData("Max fragment input components", a_limits.maxFragmentInputComponents);
    visitData("Max fragment output attachments", a_limits.maxFragmentOutputAttachments);
    visitData("Max fragment dual src attachments", a_limits.maxFragmentDualSrcAttachments);
    visitData("Max fragment combined output resources", a_limits.maxFragmentCombinedOutputResources);
    visitData("Max compute shared memory size", CapabilitiesVisitor::memorySize(a_limits.maxComputeSharedMemorySize).c_str());
    visitData("Max compute workgroup[0]", a_limits.maxComputeWorkGroupCount[0]);
    visitData("Max compute workgroup[1]", a_limits.maxComputeWorkGroupCount[1]);
    visitData("Max compute workgroup[2]", a_limits.maxComputeWorkGroupCount[2]);
    visitData("Max compute workgroup invocation", a_limits.maxComputeWorkGroupInvocations);
    visitData("Max compute workgroup size [0]", a_limits.maxComputeWorkGroupSize[0]);
    visitData("Max compute workgroup size [1]", a_limits.maxComputeWorkGroupSize[1]);
    visitData("Max compute workgroup size [2]", a_limits.maxComputeWorkGroupSize[2]);
    visitData("Sub pixel precision bits", a_limits.subPixelPrecisionBits);
    visitData("Sub texel precision bits", a_limits.subTexelPrecisionBits);
    visitData("Mipmap precision bits", a_limits.mipmapPrecisionBits);
    visitData("Max draw indexed value", a_limits.maxDrawIndexedIndexValue);
    visitData("Max draw indirect count", a_limits.maxDrawIndirectCount);
    visitData("Max sampler lod bias", a_limits.maxSamplerLodBias);
    visitData("Max sampler anisotropy", a_limits.maxSamplerAnisotropy);
    visitData("Max viewports", a_limits.maxViewports);
    visitData("Max viewport dimension [0]", a_limits.maxViewportDimensions[0]);
    visitData("Max viewport dimension [1]", a_limits.maxViewportDimensions[1]);
    visitData("viewport bounds range [0]", a_limits.viewportBoundsRange[0]);
    visitData("viewport bounds range [1]", a_limits.viewportBoundsRange[1]);
    visitData("viewport subpixel bits", a_limits.viewportSubPixelBits);
    visitData("Min memory map alignment", CapabilitiesVisitor::memorySize(a_limits.minMemoryMapAlignment).c_str());
    visitData("Min texel buffer offset alignment", CapabilitiesVisitor::memorySize(a_limits.minTexelBufferOffsetAlignment).c_str());
    visitData("Min uniform buffers offset alignment", CapabilitiesVisitor::memorySize(a_limits.minUniformBufferOffsetAlignment).c_str());
    visitData("Min storage buffer alignment", CapabilitiesVisitor::memorySize(a_limits.minStorageBufferOffsetAlignment).c_str());
    visitData("Min texel offset", a_limits.minTexelOffset);
    visitData("Max texel offset", a_limits.maxTexelOffset);
    visitData("Min texel gather offset", a_limits.minTexelGatherOffset);
    visitData("Max texel gather offset", a_limits.maxTexelGatherOffset);
    visitData("Min interpolation offset", a_limits.minInterpolationOffset);
    visitData("Max interpolation offset", a_limits.maxInterpolationOffset);
    visitData("Sub pixel interpolation offset bits", a_limits.subPixelInterpolationOffsetBits);
    visitData("Max frame buffer width", a_limits.maxFramebufferWidth);
    visitData("Max frame buffer height", a_limits.maxFramebufferHeight);
    visitData("Max frame buffer layers", a_limits.maxFramebufferLayers);
    visitData("Frame buffer color sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferColorSampleCounts).c_str());
    visitData("Frame buffer depth sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferDepthSampleCounts).c_str());
    visitData("Frame buffer stencil sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferStencilSampleCounts).c_str());
    visitData("Frame buffer ne attachment sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferNoAttachmentsSampleCounts).c_str());
    visitData("Max color attachments", a_limits.maxColorAttachments);
    visitData("Sampled image color sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageColorSampleCounts).c_str());
    visitData("Sampled image integer sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageIntegerSampleCounts).c_str());
    visitData("Sampled image depth sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageDepthSampleCounts).c_str());
    visitData("Sampled image stencil sample count", Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageStencilSampleCounts).c_str());
    visitData("Storage image sample counts", Flag<VkSampleCountFlagBits>::to_string(a_limits.storageImageSampleCounts).c_str());
    visitData("Max sample mask words", a_limits.maxSampleMaskWords);
    visitData("Timestamp compute and graphics", static_cast<bool>(a_limits.timestampComputeAndGraphics));
    visitData("Timestamp period", a_limits.timestampPeriod);
    visitData("Max clip distance", a_limits.maxClipDistances);
    visitData("Max cull distance", a_limits.maxCullDistances);
    visitData("Max combined clip and cull distances", a_limits.maxCombinedClipAndCullDistances);
    visitData("Discrete queue priorities", a_limits.discreteQueuePriorities);
    visitData("Point size range[0]", a_limits.pointSizeRange[0]);
    visitData("Point size range[1]", a_limits.pointSizeRange[1]);
    visitData("Line width range[0]", a_limits.lineWidthRange[0]);
    visitData("Line width range[1]", a_limits.lineWidthRange[1]);
    visitData("Point size granularity", a_limits.pointSizeGranularity);
    visitData("Line width granularity", a_limits.lineWidthGranularity);
    visitData("Strict lines", static_cast<bool>(a_limits.strictLines));
    visitData("Standard sample locations", static_cast<bool>(a_limits.standardSampleLocations));
    visitData("Optimal buffer copy offset alignment", CapabilitiesVisitor::memorySize(a_limits.optimalBufferCopyOffsetAlignment).c_str());
    visitData("Optimal buffer copy row pitch aligment", CapabilitiesVisitor::memorySize(a_limits.optimalBufferCopyRowPitchAlignment).c_str());
    visitData("Non coherent atom size", CapabilitiesVisitor::memorySize(a_limits.nonCoherentAtomSize).c_str());
}

void CapabilitiesVisitor::visit(const VkPhysicalDeviceSparseProperties& a_sparseProp)
{
    visitData("Residency standard 2D block shape", static_cast<bool>(a_sparseProp.residencyStandard2DBlockShape));
    visitData("Residency standard 2D multisample block shape", static_cast<bool>(a_sparseProp.residencyStandard2DMultisampleBlockShape));
    visitData("Residency standard 3D block shape", static_cast<bool>(a_sparseProp.residencyStandard3DBlockShape));
    visitData("Residency aligned mip size", static_cast<bool>(a_sparseProp.residencyAlignedMipSize));
    visitData("Residency non resident strict", static_cast<bool>(a_sparseProp.residencyNonResidentStrict));
}

void CapabilitiesVisitor::visit(const VkPhysicalDeviceFeatures& a_features)
{
    visitData("Robust buffer access", static_cast<bool>(a_features.robustBufferAccess));
    visitData("Full draw index uint32", static_cast<bool>(a_features.fullDrawIndexUint32));
    visitData("Image cube array", static_cast<bool>(a_features.imageCubeArray));
    visitData("Independent blend", static_cast<bool>(a_features.independentBlend));
    visitData("Geometry shader", static_cast<bool>(a_features.geometryShader));
    visitData("Teselation shader", static_cast<bool>(a_features.tessellationShader));
    visitData("Sample rate shading", static_cast<bool>(a_features.sampleRateShading));
    visitData("Dual src blend", static_cast<bool>(a_features.dualSrcBlend));
    visitData("Logic Op", static_cast<bool>(a_features.logicOp));
    visitData("Multi draw indirect", static_cast<bool>(a_features.multiDrawIndirect));
    visitData("Draw indirect first instance", static_cast<bool>(a_features.drawIndirectFirstInstance));
    visitData("Depth clamp", static_cast<bool>(a_features.depthClamp));
    visitData("Depth bias clamp", static_cast<bool>(a_features.depthBiasClamp));
    visitData("Fill mode non solid", static_cast<bool>(a_features.fillModeNonSolid));
    visitData("Depthh bounds", static_cast<bool>(a_features.depthBounds));
    visitData("Wide lines", static_cast<bool>(a_features.wideLines));
    visitData("Large points", static_cast<bool>(a_features.largePoints));
    visitData("Alpha to one", static_cast<bool>(a_features.alphaToOne));
    visitData("Multi viewport", static_cast<bool>(a_features.multiViewport));
    visitData("Sampler anisotropy", static_cast<bool>(a_features.samplerAnisotropy));
    visitData("Texture compression ETC2", static_cast<bool>(a_features.textureCompressionETC2));
    visitData("Texture compression ASTC_LDR", static_cast<bool>(a_features.textureCompressionASTC_LDR));
    visitData("Texture compression BC", static_cast<bool>(a_features.textureCompressionBC));
    visitData("Occlusion query precise", static_cast<bool>(a_features.occlusionQueryPrecise));
    visitData("Pipeline statics query", static_cast<bool>(a_features.pipelineStatisticsQuery));
    visitData("Vertex pipeline stores and atomics", static_cast<bool>(a_features.vertexPipelineStoresAndAtomics));
    visitData("Fragment stores and atomics", static_cast<bool>(a_features.fragmentStoresAndAtomics));
    visitData("Shader tesselation and geometry point size", static_cast<bool>(a_features.shaderTessellationAndGeometryPointSize));
    visitData("Shader image gather extended", static_cast<bool>(a_features.shaderImageGatherExtended));
    visitData("Shader storage image extended formats", static_cast<bool>(a_features.shaderStorageImageExtendedFormats));
    visitData("Shader storage image multisample", static_cast<bool>(a_features.shaderStorageImageMultisample));
    visitData("Shader storage image read without format", static_cast<bool>(a_features.shaderStorageImageReadWithoutFormat));
    visitData("Shader storage image write without format", static_cast<bool>(a_features.shaderStorageImageWriteWithoutFormat));
    visitData("Shader uniform buffer array dynamic indexing", static_cast<bool>(a_features.shaderUniformBufferArrayDynamicIndexing));
    visitData("Shader ampled image array dynamic indexing", static_cast<bool>(a_features.shaderSampledImageArrayDynamicIndexing));
    visitData("Shader storage buffer array dynamic indexing", static_cast<bool>(a_features.shaderStorageBufferArrayDynamicIndexing));
    visitData("Shader storage image array dynamic indexing", static_cast<bool>(a_features.shaderStorageImageArrayDynamicIndexing));
    visitData("Shader clip distance", static_cast<bool>(a_features.shaderClipDistance));
    visitData("Shader cull distance", static_cast<bool>(a_features.shaderCullDistance));
    visitData("Shader Float 64", static_cast<bool>(a_features.shaderFloat64));
    visitData("Shader Int 64", static_cast<bool>(a_features.shaderInt64));
    visitData("Shader Int 16", static_cast<bool>(a_features.shaderInt16));
    visitData("Shader resource residency", static_cast<bool>(a_features.shaderResourceResidency));
    visitData("Shader resource min lod", static_cast<bool>(a_features.shaderResourceMinLod));
    visitData("Sparse binding", static_cast<bool>(a_features.sparseBinding));
    visitData("Sparse residency buffer", static_cast<bool>(a_features.sparseResidencyBuffer));
    visitData("Sparse residency image 2D", static_cast<bool>(a_features.sparseResidencyImage2D));
    visitData("Sparse residency image 3D", static_cast<bool>(a_features.sparseResidencyImage3D));
    visitData("Sparse residency 2 samples", static_cast<bool>(a_features.sparseResidency2Samples));
    visitData("Sparse residency 4 samples", static_cast<bool>(a_features.sparseResidency4Samples));
    visitData("Sparse residency 8 samples", static_cast<bool>(a_features.sparseResidency8Samples));
    visitData("Sparse residency 16 samples", static_cast<bool>(a_features.sparseResidency16Samples));
    visitData("Sparse residency aliased", static_cast<bool>(a_features.sparseResidencyAliased));
    visitData("Variable multisample rate", static_cast<bool>(a_features.variableMultisampleRate));
    visitData("Inherited queries", static_cast<bool>(a_features.inheritedQueries));
}