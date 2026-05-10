#include "pch.h"
#include "EngineRenderer.h"
#include "EngineDevice.h"


EngineRenderer::EngineRenderer(EngineDevicePtr a_device, const SurfaceConfiguration& a_surfConf) :
	m_device{ a_device }, m_swapChain{ a_device->deviceContext(), a_surfConf }
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