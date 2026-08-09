#include "pch.h"
#include "EngineTexture.h"

EngineTexture::EngineTexture(const DeviceContext& a_ctxt, const TextureParameters& a_params) : m_devCtx{ a_ctxt },
    m_parameters{ a_params }
{
    // to parametrize
    VkImageCreateInfo imageInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = m_parameters.format,
        .extent = {m_parameters.width, m_parameters.height,  1},
        .mipLevels = m_parameters.mipmap,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage =
            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VmaAllocationCreateInfo allocationInfo{
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    VK_CHECK_LOG(vmaCreateImage(m_devCtx.m_memAllocator, &imageInfo, &allocationInfo, &m_image, &m_allocation, nullptr));
}

EngineTexture::~EngineTexture()
{
    if (m_image)
    {
        vmaDestroyImage(m_devCtx.m_memAllocator, m_image, m_allocation);
        m_image = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
    }
}