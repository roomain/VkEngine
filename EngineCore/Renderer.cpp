#include "pch.h"
#include "Renderer.h"
#include "Device.h"


Renderer::Renderer(DevicePtr a_device, const SurfaceConfiguration& a_surfConf) :
	m_device{ a_device }, m_swapChain{ a_device->deviceContext(), a_surfConf }
{
	//
}

Renderer::~Renderer()
{
	//
}

void Renderer::resize(const uint32_t a_width, const uint32_t a_height)
{
	m_swapChain.resize(a_width, a_height);
}