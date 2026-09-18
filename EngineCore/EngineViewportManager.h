#pragma once
/***********************************************
* @headerfile EngineViewportManager.h
* @date 09 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include "iterators.h"
#include "EngineViewport.h"
#include "enginecore_globals.h"

using EngineViewportPtr = std::shared_ptr<EngineViewport>;

#pragma warning(push)
#pragma warning( disable : 4251 )

class ENGINECORE_EXPORT EngineViewportManager
{
private:
	unsigned int m_resX = 0;
	unsigned int m_resY = 0;
	std::vector<EngineViewportPtr> m_viewports;

public:
	EngineViewportManager() = delete;
	explicit EngineViewportManager(const unsigned int a_resX, const unsigned int a_resY);
	virtual ~EngineViewportManager() = default;
	EngineViewportPtr getViewport(const unsigned int a_posX, const unsigned int a_posY)const;
	DEFINE_ITER(std::vector<EngineViewportPtr>, m_viewports)
	DEFINE_CONST_ITER(std::vector<EngineViewportPtr>, m_viewports)
	/*@brief update windows resolution*/
	void update(const unsigned int a_resX, const unsigned int a_resY);	
	void grab(const float a_posX, const float a_resY, std::vector<ViewportTransform>& a_grab)const;

	// todo add/remove viewports
	
};
#pragma warning(pop)
