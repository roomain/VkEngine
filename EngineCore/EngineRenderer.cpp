#include "pch.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"


EngineRenderer::EngineRenderer(EngineDevicePtr a_device, VkSurfaceKHR a_surface, const uint32_t a_width, const uint32_t a_height) :
	m_device{ a_device }, m_swapChain{ a_device->deviceContext(), a_surface , a_width, a_height }
{
	//
}

EngineRenderer::~EngineRenderer()
{
	//
}

void EngineRenderer::resize(const uint32_t a_width, const uint32_t a_height)
{
	m_swapChain.resize(a_width, a_height);
}