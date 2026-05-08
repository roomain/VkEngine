#pragma once
/***********************************************
* @headerfile EngineRenderer.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include "EngineSwapChain.h"

class EngineDevice;
using EngineDevicePtr = std::shared_ptr<EngineDevice>;

/*@brief Renderer: for specific surface, shared device*/
class EngineRenderer
{
	friend class EngineApplication;
private:
	EngineDevicePtr m_device;		/*!< device used by renderer*/
	EngineSwapChain m_swapChain;	/*!< swapchain*/

	EngineRenderer(EngineDevicePtr a_device, VkSurfaceKHR a_surface, const uint32_t a_width, const uint32_t a_height);

public:
	NOT_COPIABLE(EngineRenderer)
	virtual ~EngineRenderer();
	void resize(const uint32_t a_width, const uint32_t a_height);
	[[nodiscard]] EngineDevicePtr device()const { return m_device; }
};

