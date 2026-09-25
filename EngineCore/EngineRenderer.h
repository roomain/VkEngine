#pragma once
/***********************************************
* @headerfile EngineRenderer.h
* @date 15 / 03 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "EngineSwapChain.h"
#include "enginecore_globals.h"

struct SurfaceConfiguration;

class EngineDevice;
using EngineDevicePtr = std::shared_ptr<EngineDevice>;

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief Renderer: for specific surface, shared device*/
class ENGINECORE_EXPORT EngineRenderer
{
	friend class EngineApplication;
private:
	EngineDevicePtr m_device;		/*!< device used by renderer*/
	EngineSwapChain m_swapChain;	/*!< swapchain*/

	EngineRenderer(EngineDevicePtr a_device, const SurfaceConfiguration& a_surfConf);

public:
	NOT_COPIABLE(EngineRenderer)
	virtual ~EngineRenderer();
	void resize(const uint32_t a_width, const uint32_t a_height);
	[[nodiscard]] EngineDevicePtr device()const { return m_device; }
};
#pragma warning(pop)

