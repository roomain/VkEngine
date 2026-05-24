#include "EditDeviceFeatures.h"

EditDeviceFeatures::EditDeviceFeatures(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	DeviceFeatures features;
	ui.robustBufferAccess->setChecked(features.robustBufferAccess);
	ui.fullDrawIndexUint32->setChecked(features.fullDrawIndexUint32);
	ui.imageCubeArray->setChecked(features.imageCubeArray);
	ui.independentBlend->setChecked(features.independentBlend);
	ui.geometryShader->setChecked(features.geometryShader);
	ui.tessellationShader->setChecked(features.tessellationShader);
	ui.sampleRateShading->setChecked(features.sampleRateShading);
	ui.dualSrcBlend->setChecked(features.dualSrcBlend);
	ui.logicOp->setChecked(features.logicOp);
	ui.multiDrawIndirect->setChecked(features.multiDrawIndirect);
	ui.drawIndirectFirstInstance->setChecked(features.drawIndirectFirstInstance);
	ui.depthClamp->setChecked(features.depthClamp);
	ui.depthBiasClamp->setChecked(features.depthBiasClamp);
	ui.fillModeNonSolid->setChecked(features.fillModeNonSolid);
	ui.depthBounds->setChecked(features.depthBounds);
	ui.wideLines->setChecked(features.wideLines);
	ui.largePoints->setChecked(features.largePoints);
	ui.alphaToOne->setChecked(features.alphaToOne);
	ui.multiViewport->setChecked(features.multiViewport);
	ui.samplerAnisotropy->setChecked(features.samplerAnisotropy);
	ui.textureCompressionETC2->setChecked(features.textureCompressionETC2);
	ui.textureCompressionASTC_LDR->setChecked(features.textureCompressionASTC_LDR);
	ui.textureCompressionBC->setChecked(features.textureCompressionBC);
	ui.occlusionQueryPrecise->setChecked(features.occlusionQueryPrecise);
	ui.pipelineStatisticsQuery->setChecked(features.pipelineStatisticsQuery);
	ui.vertexPipelineStoresAndAtomics->setChecked(features.vertexPipelineStoresAndAtomics);
	ui.fragmentStoresAndAtomics->setChecked(features.fragmentStoresAndAtomics);
	ui.shaderTessellationAndGeometryPointSize->setChecked(features.shaderTessellationAndGeometryPointSize);
	ui.shaderImageGatherExtended->setChecked(features.shaderImageGatherExtended);
	ui.shaderStorageImageExtendedFormats->setChecked(features.shaderStorageImageExtendedFormats);
	ui.shaderStorageImageMultisample->setChecked(features.shaderStorageImageMultisample);
	ui.shaderStorageImageReadWithoutFormat->setChecked(features.shaderStorageImageReadWithoutFormat);
	ui.shaderStorageImageWriteWithoutFormat->setChecked(features.shaderStorageImageWriteWithoutFormat);
	ui.shaderUniformBufferArrayDynamicIndexing->setChecked(features.shaderUniformBufferArrayDynamicIndexing);
	ui.shaderSampledImageArrayDynamicIndexing->setChecked(features.shaderSampledImageArrayDynamicIndexing);
	ui.shaderStorageBufferArrayDynamicIndexing->setChecked(features.shaderStorageBufferArrayDynamicIndexing);
	ui.shaderStorageImageArrayDynamicIndexing->setChecked(features.shaderStorageImageArrayDynamicIndexing);
	ui.shaderClipDistance->setChecked(features.shaderClipDistance);
	ui.shaderCullDistance->setChecked(features.shaderCullDistance);
	ui.shaderFloat64->setChecked(features.shaderFloat64);
	ui.shaderInt64->setChecked(features.shaderInt64);
	ui.shaderInt16->setChecked(features.shaderInt16);
	ui.shaderResourceResidency->setChecked(features.shaderResourceResidency);
	ui.shaderResourceMinLod->setChecked(features.shaderResourceMinLod);
	ui.sparseBinding->setChecked(features.sparseBinding);
	ui.sparseResidencyBuffer->setChecked(features.sparseResidencyBuffer);
	ui.sparseResidencyImage2D->setChecked(features.sparseResidencyImage2D);
	ui.sparseResidencyImage3D->setChecked(features.sparseResidencyImage3D);
	ui.sparseResidency2Samples->setChecked(features.sparseResidency2Samples);
	ui.sparseResidency4Samples->setChecked(features.sparseResidency4Samples);
	ui.sparseResidency8Samples->setChecked(features.sparseResidency8Samples);
	ui.sparseResidency16Samples->setChecked(features.sparseResidency16Samples);
	ui.sparseResidencyAliased->setChecked(features.sparseResidencyAliased);
	ui.variableMultisampleRate->setChecked(features.variableMultisampleRate);
	ui.inheritedQueries->setChecked(features.inheritedQueries);
}

EditDeviceFeatures::~EditDeviceFeatures()
{}


DeviceFeatures EditDeviceFeatures::get()
{
	DeviceFeatures features;
	features.robustBufferAccess = ui.robustBufferAccess->checkState() == Qt::Checked;
	features.fullDrawIndexUint32 = ui.fullDrawIndexUint32->checkState() == Qt::Checked;
	features.imageCubeArray = ui.imageCubeArray->checkState() == Qt::Checked;
	features.independentBlend = ui.independentBlend->checkState() == Qt::Checked;
	features.geometryShader = ui.geometryShader->checkState() == Qt::Checked;
	features.tessellationShader = ui.tessellationShader->checkState() == Qt::Checked;
	features.sampleRateShading = ui.sampleRateShading->checkState() == Qt::Checked;
	features.dualSrcBlend = ui.dualSrcBlend->checkState() == Qt::Checked;
	features.logicOp = ui.logicOp->checkState() == Qt::Checked;
	features.multiDrawIndirect = ui.multiDrawIndirect->checkState() == Qt::Checked;
	features.drawIndirectFirstInstance = ui.drawIndirectFirstInstance->checkState() == Qt::Checked;
	features.depthClamp = ui.depthClamp->checkState() == Qt::Checked;
	features.depthBiasClamp = ui.depthBiasClamp->checkState() == Qt::Checked;
	features.fillModeNonSolid = ui.fillModeNonSolid->checkState() == Qt::Checked;
	features.depthBounds = ui.depthBounds->checkState() == Qt::Checked;
	features.wideLines = ui.wideLines->checkState() == Qt::Checked;
	features.largePoints = ui.largePoints->checkState() == Qt::Checked;
	features.alphaToOne = ui.alphaToOne->checkState() == Qt::Checked;
	features.multiViewport = ui.multiViewport->checkState() == Qt::Checked;
	features.samplerAnisotropy = ui.samplerAnisotropy->checkState() == Qt::Checked;
	features.textureCompressionETC2 = ui.textureCompressionETC2->checkState() == Qt::Checked;
	features.textureCompressionASTC_LDR = ui.textureCompressionASTC_LDR->checkState() == Qt::Checked;
	features.textureCompressionBC = ui.textureCompressionBC->checkState() == Qt::Checked;
	features.occlusionQueryPrecise = ui.occlusionQueryPrecise->checkState() == Qt::Checked;
	features.pipelineStatisticsQuery = ui.pipelineStatisticsQuery->checkState() == Qt::Checked;
	features.vertexPipelineStoresAndAtomics = ui.vertexPipelineStoresAndAtomics->checkState() == Qt::Checked;
	features.fragmentStoresAndAtomics = ui.fragmentStoresAndAtomics->checkState() == Qt::Checked;
	features.shaderTessellationAndGeometryPointSize = ui.shaderTessellationAndGeometryPointSize->checkState() == Qt::Checked;
	features.shaderImageGatherExtended = ui.shaderImageGatherExtended->checkState() == Qt::Checked;
	features.shaderStorageImageExtendedFormats = ui.shaderStorageImageExtendedFormats->checkState() == Qt::Checked;
	features.shaderStorageImageMultisample = ui.shaderStorageImageMultisample->checkState() == Qt::Checked;
	features.shaderStorageImageReadWithoutFormat = ui.shaderStorageImageReadWithoutFormat->checkState() == Qt::Checked;
	features.shaderStorageImageWriteWithoutFormat = ui.shaderStorageImageWriteWithoutFormat->checkState() == Qt::Checked;
	features.shaderUniformBufferArrayDynamicIndexing = ui.shaderUniformBufferArrayDynamicIndexing->checkState() == Qt::Checked;
	features.shaderSampledImageArrayDynamicIndexing = ui.shaderSampledImageArrayDynamicIndexing->checkState() == Qt::Checked;
	features.shaderStorageBufferArrayDynamicIndexing = ui.shaderStorageBufferArrayDynamicIndexing->checkState() == Qt::Checked;
	features.shaderStorageImageArrayDynamicIndexing = ui.shaderStorageImageArrayDynamicIndexing->checkState() == Qt::Checked;
	features.shaderClipDistance = ui.shaderClipDistance->checkState() == Qt::Checked;
	features.shaderCullDistance = ui.shaderCullDistance->checkState() == Qt::Checked;
	features.shaderFloat64 = ui.shaderFloat64->checkState() == Qt::Checked;
	features.shaderInt64 = ui.shaderInt64->checkState() == Qt::Checked;
	features.shaderInt16 = ui.shaderInt16->checkState() == Qt::Checked;
	features.shaderResourceResidency = ui.shaderResourceResidency->checkState() == Qt::Checked;
	features.shaderResourceMinLod = ui.shaderResourceMinLod->checkState() == Qt::Checked;
	features.sparseBinding = ui.sparseBinding->checkState() == Qt::Checked;
	features.sparseResidencyBuffer = ui.sparseResidencyBuffer->checkState() == Qt::Checked;
	features.sparseResidencyImage2D = ui.sparseResidencyImage2D->checkState() == Qt::Checked;
	features.sparseResidencyImage3D = ui.sparseResidencyImage3D->checkState() == Qt::Checked;
	features.sparseResidency2Samples = ui.sparseResidency2Samples->checkState() == Qt::Checked;
	features.sparseResidency4Samples = ui.sparseResidency4Samples->checkState() == Qt::Checked;
	features.sparseResidency8Samples = ui.sparseResidency8Samples->checkState() == Qt::Checked;
	features.sparseResidency16Samples = ui.sparseResidency16Samples->checkState() == Qt::Checked;
	features.sparseResidencyAliased = ui.sparseResidencyAliased->checkState() == Qt::Checked;
	features.variableMultisampleRate = ui.variableMultisampleRate->checkState() == Qt::Checked;
	features.inheritedQueries = ui.inheritedQueries->checkState() == Qt::Checked;
	return features;
}