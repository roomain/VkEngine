#pragma once
/***********************************************
* @headerfile EngineParameters.h
* @date 28 / 03 / 2026
* @author Roomain
************************************************/
#include <vector>
#include <string>
#include <vulkan/vulkan.hpp>
#include "string_utils.h"
#include "Reflective_macros.h"
#include "ReflectiveException.h"
#include "reflective_convert.h"
#include "EngineParameters.generated.h"

REFLECT_CLASS
struct BaseParameters
{
	std::vector<std::string> extensions;
	std::vector<std::string> layers;
	REFLECT_DEFINE(BaseParameters);
};

REFLECT_CLASS
struct EngineParameters : BaseParameters
{
	bool debugging = false;
	REFLECT_DEFINE(EngineParameters);
};



//-----------------------------------------------------------------------------------------------
REFLECT_CLASS
struct DeviceFeatures 
{
    bool robustBufferAccess = false;
    bool fullDrawIndexUint32 = false;
    bool imageCubeArray = false;
    bool independentBlend = false;
    bool geometryShader = false;
    bool tessellationShader = false;
    bool sampleRateShading = false;
    bool dualSrcBlend = false;
    bool logicOp = false;
    bool multiDrawIndirect = false;
    bool drawIndirectFirstInstance = false;
    bool depthClamp = false;
    bool depthBiasClamp = false;
    bool fillModeNonSolid = false;
    bool depthBounds = false;
    bool wideLines = false;
    bool largePoints = false;
    bool alphaToOne = false;
    bool multiViewport = false;
    bool samplerAnisotropy = false;
    bool textureCompressionETC2 = false;
    bool textureCompressionASTC_LDR = false;
    bool textureCompressionBC = false;
    bool occlusionQueryPrecise = false;
    bool pipelineStatisticsQuery = false;
    bool vertexPipelineStoresAndAtomics = false;
    bool fragmentStoresAndAtomics = false;
    bool shaderTessellationAndGeometryPointSize = false;
    bool shaderImageGatherExtended = false;
    bool shaderStorageImageExtendedFormats = false;
    bool shaderStorageImageMultisample = false;
    bool shaderStorageImageReadWithoutFormat = false;
    bool shaderStorageImageWriteWithoutFormat = false;
    bool shaderUniformBufferArrayDynamicIndexing = false;
    bool shaderSampledImageArrayDynamicIndexing = false;
    bool shaderStorageBufferArrayDynamicIndexing = false;
    bool shaderStorageImageArrayDynamicIndexing = false;
    bool shaderClipDistance = false;
    bool shaderCullDistance = false;
    bool shaderFloat64 = false;
    bool shaderInt64 = false;
    bool shaderInt16 = false;
    bool shaderResourceResidency = false;
    bool shaderResourceMinLod = false;
    bool sparseBinding = false;
    bool sparseResidencyBuffer = false;
    bool sparseResidencyImage2D = false;
    bool sparseResidencyImage3D = false;
    bool sparseResidency2Samples = false;
    bool sparseResidency4Samples = false;
    bool sparseResidency8Samples = false;
    bool sparseResidency16Samples = false;
    bool sparseResidencyAliased = false;
    bool variableMultisampleRate = false;
    bool inheritedQueries = false;
    REFLECT_DEFINE(DeviceFeatures);
};

constexpr VkQueueFlagBits operator | (const VkQueueFlagBits a_first, const VkQueueFlagBits a_second)
{
    return static_cast<VkQueueFlagBits>(static_cast<unsigned int>(a_first) | static_cast<unsigned int>(a_second));
}

REFLECT_CLASS
struct QueuesParameters
{
    VkQueueFlagBits flags = static_cast<VkQueueFlagBits>(0);
    uint32_t count = 1;
    float priority = 1;
    REFLECT_DEFINE(QueuesParameters);
};


REFLECT_CLASS
struct DeviceParameters : BaseParameters
{
    DeviceFeatures features;
    std::vector<QueuesParameters> queues;
	REFLECT_DEFINE(DeviceParameters);
};

/*@brief tool to remove doubloon*/
template<typename Type>
inline std::vector<Type> removeDoubloon(const std::vector<Type>& a_data)
{
    std::vector<Type> cleaned;
    for (const auto& data : a_data)
    {
        if (auto iter = std::ranges::find(cleaned, data); iter == cleaned.cend())
            cleaned.emplace_back(data);
    }
    return cleaned;
}