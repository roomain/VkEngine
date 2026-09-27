#pragma once
/***********************************************
* @headerfile VulkanPipelineInitializer.h
* @date 27 / 09 / 2026
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>

constexpr [[nodiscard]] VkPipelineVertexInputStateCreateInfo createPipelineVertexInputState()
{
	return VkPipelineVertexInputStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr 
	};
}

constexpr [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo createPipelineAssemblyState(const VkPrimitiveTopology a_topology,
	const VkPipelineInputAssemblyStateCreateFlags a_flags,
	const VkBool32 a_primitiveRestartEnable)
{
	return VkPipelineInputAssemblyStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.topology = a_topology,
		.primitiveRestartEnable = a_primitiveRestartEnable,
	};
}

constexpr [[nodiscard]] VkPipelineTessellationStateCreateInfo createPipelineTesselationState(
	const VkPipelineTessellationStateCreateFlags a_flags,
	const uint32_t a_controlPoints)
{
	return VkPipelineTessellationStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.patchControlPoints = a_controlPoints
	};
}

constexpr [[nodiscard]] VkPipelineRasterizationStateCreateInfo createPipelineRasterizationState(
	const VkPipelineRasterizationStateCreateFlags a_flags)
{
	return VkPipelineRasterizationStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0,
		.depthBiasClamp = 0,
		.depthBiasSlopeFactor = 0,
		.lineWidth = 0
	};
}

constexpr [[nodiscard]] VkPipelineMultisampleStateCreateInfo createPipelineMultisampleState(
	const VkPipelineMultisampleStateCreateFlags a_flags,
	const VkSampleCountFlagBits a_sampleFlag)
{
	return VkPipelineMultisampleStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.rasterizationSamples = a_sampleFlag,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = .0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
	};
}

constexpr [[nodiscard]] VkPipelineDepthStencilStateCreateInfo createPipelineDepthStencilState(
	const VkPipelineDepthStencilStateCreateFlags a_flag
	)
{
	return VkPipelineDepthStencilStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flag,
		.depthTestEnable = VK_FALSE,
		.depthWriteEnable = VK_FALSE,
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
		.minDepthBounds = .0f,
		.maxDepthBounds = .0f
	};
}

constexpr [[nodiscard]] VkPipelineColorBlendStateCreateInfo createPipelineColorBlendState(
	const VkPipelineColorBlendStateCreateFlags a_flag)
{
	return VkPipelineColorBlendStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flag,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_CLEAR,
		.attachmentCount = 0,
		.pAttachments = nullptr
	};
};

template<typename DynStateContainer>
constexpr [[nodiscard]] VkPipelineDynamicStateCreateInfo createPipeline(
	const VkPipelineDynamicStateCreateFlags a_flags,
	const DynStateContainer& a_container)
{
	return VkPipelineDynamicStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.dynamicStateCount = static_cast<uint32_t>(a_container.size()),
		.pDynamicStates = a_container.data()
	};
}

template<typename StageContainer>
constexpr [[nodiscard]] VkGraphicsPipelineCreateInfo createGraphicPipeline(
	const VkPipelineCreateFlags a_flags,
	const StageContainer& a_stages)
{
	return VkGraphicsPipelineCreateInfo {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags;
		.stageCount = static_cast<uint32_t>(a_stages.size()),
		.pStages = a_stages.data(),
		.pVertexInputState = nullptr,
		.pInputAssemblyState = nullptr,
		.pTessellationState = nullptr,
		.pViewportState = nullptr,
		.pRasterizationState = nullptr,
		.pMultisampleState = nullptr,
		.pDepthStencilState = nullptr,
		.pColorBlendState = nullptr,
		.pDynamicState = nullptr,
		.layout = VK_NULL_HANDLE,
		.renderPass = VK_NULL_HANDLE,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = 0
	};
};


constexpr [[nodiscard]] VkComputePipelineCreateInfo createComputePipeline(const VkPipelineCreateFlags a_flag)
{
	return VkComputePipelineCreateInfo {
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flag,
		.layout = VK_NULL_HANDLE,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = 0
	};
}