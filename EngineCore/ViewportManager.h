#pragma once
/***********************************************
* @headerfile ViewportManager.h
* @date 09 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include "iterators.h"
#include "Viewport.h"
#include "enginecore_globals.h"

using ViewportPtr = std::shared_ptr<Viewport>;

#pragma warning(push)
#pragma warning( disable : 4251 )

class ENGINECORE_EXPORT ViewportManager
{
private:
	unsigned int m_resX = 0;
	unsigned int m_resY = 0;
	std::vector<ViewportPtr> m_viewports;

public:
	ViewportManager() = delete;
	explicit ViewportManager(const unsigned int a_resX, const unsigned int a_resY);
	virtual ~ViewportManager() = default;
	ViewportPtr getViewport(const unsigned int a_posX, const unsigned int a_posY)const;
	DEFINE_ITER(std::vector<ViewportPtr>, m_viewports)
	DEFINE_CONST_ITER(std::vector<ViewportPtr>, m_viewports)
	/*@brief update windows resolution*/
	void update(const unsigned int a_resX, const unsigned int a_resY);	
	void grab(const float a_posX, const float a_resY, std::vector<ViewportTransform>& a_grab)const;

	// todo add/remove viewports
	
};
#pragma warning(pop)
