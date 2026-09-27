#pragma once
/***********************************************
* @headerfile Renderer.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "SwapChain.h"
#include "enginecore_globals.h"

struct SurfaceConfiguration;

class Device;
using DevicePtr = std::shared_ptr<Device>;

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief Renderer: for specific surface, shared device*/
class ENGINECORE_EXPORT Renderer
{
	friend class Application;
private:
	DevicePtr m_device;		/*!< device used by renderer*/
	SwapChain m_swapChain;	/*!< swapchain*/

	Renderer(DevicePtr a_device, const SurfaceConfiguration& a_surfConf);

public:
	NOT_COPIABLE(Renderer)
	virtual ~Renderer();
	void resize(const uint32_t a_width, const uint32_t a_height);
	[[nodiscard]] DevicePtr device()const { return m_device; }
};
#pragma warning(pop)

