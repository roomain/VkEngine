#pragma once
/***********************************************
* @headerfile EngineTexture.h
* @date 09 / 08 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "DeviceContext.h"
#include "notCopiable.h"
#include "enginecore_globals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

struct TextureParameters
{
	uint32_t width = 0;							/*!< image width*/
	uint32_t height = 0;						/*!< image height*/
	uint32_t mipmap = 1;						/*!< mipmap*/
	VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;	/*!< format*/
};

/*@brief class for using texture with vulkan*/
class ENGINECORE_EXPORT EngineTexture
{
	friend class EngineDevice;
private:
	DeviceContext m_devCtx;							/*!< device context*/
	VmaAllocation m_allocation = VK_NULL_HANDLE;	/*!< vma allocation*/
	VkImage m_image = VK_NULL_HANDLE;				/*!< vulkan image*/
	TextureParameters m_parameters;					/*!< texture parameters*/

	// to parametrize
	EngineTexture(const DeviceContext& a_ctxt, const TextureParameters& a_params);

public:
	NOT_COPIABLE(EngineTexture)
	EngineTexture() = delete;
	virtual ~EngineTexture();

	constexpr [[nodiscard]] VkFormat format()const { return m_parameters.format; }
	constexpr [[nodiscard]] uint32_t width()const { return m_parameters.width; }
	constexpr [[nodiscard]] uint32_t height()const { return m_parameters.height; }
	constexpr [[nodiscard]] VkImage imageData()const { return m_image; }

};

using EngineTexturePtr = std::shared_ptr<EngineTexture>;

#pragma warning(pop)