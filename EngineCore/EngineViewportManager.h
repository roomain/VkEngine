#pragma once
/***********************************************
* @headerfile EngineViewportManager.h
* @date 09 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <functional>
#include "iterators.h"
#include "enginecore_globals.h"

class EngineViewport;
using EngineViewportPtr = std::shared_ptr<EngineViewport>;

/*@brief viewport transformation callback*/
using ViewportTransform = std::function<void(const float)>;

enum class GrabMode
{
	Vertical = 0,
	Horizontal
};


struct GrapViewport
{
	bool valid = false;
	float position = 0;
	GrabMode mode;
	std::vector<ViewportTransform> m_viewportCallback
};

class ENGINECORE_EXPORT EngineViewportManager
{
private:
	unsigned int m_resX = 0;
	unsigned int m_resY = 0;
	std::vector<EngineViewportPtr> m_viewports;

public:
	EngineViewportManager() = default;
	virtual ~EngineViewportManager() = default;
	EngineViewportPtr getViewport(const unsigned int a_posX, const unsigned int a_Y)const;
	DEFINE_ITER(std::vector<EngineViewportPtr>, m_viewports)
	DEFINE_CONST_ITER(std::vector<EngineViewportPtr>, m_viewports)
	void update(const unsigned int a_resX, const unsigned int a_resY);	
	bool grab(const float a_resX, const float a_resY, GrapViewport& a_grab);
};

#include <cmath>
#include <cstdlib>
#include <vulkan/vulkan.hpp>
class EngineViewport
{
private:
	VkViewport m_viewport;

public:
	EngineViewport() = delete;
	EngineViewport(const float a_posX, const float a_posY, const float a_width, const float a_height) :
		m_viewport{ a_posX, a_posY, a_width, a_height, .0f, 1.0f }
	{
		//
	}

	inline void setDepth(const float a_min, const float a_max)
	{
		m_viewport.minDepth = a_min;
		m_viewport.maxDepth = a_max;
	}

	inline void resize(const float a_width, const float a_height)
	{
		m_viewport.width = a_width;
		m_viewport.height = a_height;
	}

	inline void moveTo(const float a_posX, const float a_posY)
	{
		m_viewport.x = a_posX;
		m_viewport.y = a_posY;
	}

	inline void translate(const float a_transX, const float a_transY)
	{
		m_viewport.x += a_transX;
		m_viewport.y += a_transY;
	}

	inline [[nodiscard]] const VkViewport& viewport()const { return m_viewport; }
	
	inline [[nodiscard]] bool grab(const float a_posX, const float a_posY, GrapViewport& a_grab)
	{
		static constexpr float EPSILON = 3.f;
		a_grab.valid = false;
		if (fabsf(a_posX - m_viewport.x) < EPSILON)
		{
			a_grab.valid = true;
			a_grab.mode = GrabMode::Vertical;
			a_grab.position = m_viewport.x;
			a_grab.m_viewportCallback.emplace_back([this](const float a_translation)
				{
					// todo
				});
		}
		else if (fabsf(a_posX - m_viewport.x - m_viewport.width) < EPSILON)
		{
			a_grab.valid = true;
			a_grab.mode = GrabMode::Vertical;
			a_grab.position = m_viewport.x + m_viewport.width;
			a_grab.m_viewportCallback.emplace_back([this](const float a_translation)
				{
					// todo
				});
		}
		else if (fabsf(a_posY - m_viewport.y) < EPSILON)
		{
			a_grab.valid = true;
			a_grab.mode = GrabMode::Horizontal;
			a_grab.position = m_viewport.y;
			a_grab.m_viewportCallback.emplace_back([this](const float a_translation)
				{
					// todo
				});
		}
		else if (fabsf(a_posY - m_viewport.y - m_viewport.height) < EPSILON)
		{
			a_grab.valid = true;
			a_grab.mode = GrabMode::Horizontal;
			a_grab.position = m_viewport.y + m_viewport.height;
			a_grab.m_viewportCallback.emplace_back([this](const float a_translation)
				{
					// todo
				});
		}
		return a_grab.valid;
	}

	inline [[nodiscard]] bool grab(const float a_translation, const GrapViewport& a_grab)
	{
		static constexpr float EPSILON = 3.f;
		switch (a_grab.mode)
		{
		case GrabMode::Horizontal:
			if ()
			{
				//
			}
			else if ()
			{
				//
			}
			break;

		case GrabMode::Vertical:
			break;
		}
		return false;
	}

	constexpr [[nodiscard]]  bool isIn(const float a_posX, const float a_posY)
	{
		return a_posX >= m_viewport.x && a_posX <= (m_viewport.x + m_viewport.width) &&
			a_posY >= m_viewport.y && a_posY <= (m_viewport.y + m_viewport.height);
	}
};