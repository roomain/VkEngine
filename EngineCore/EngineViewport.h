#pragma once
/***********************************************
* @headerfile EngineViewport.h
* @date 09 / 09 / 2026
* @author Roomain
************************************************/
#include <cmath>
#include <functional>
#include <vulkan/vulkan.hpp>


/*@brief viewport transformation callback*/
using ViewportTransform = std::function<bool(const float)>;

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

	inline [[nodiscard]] bool grab(const float a_posX, const float a_posY, ViewportTransform& a_grabFun)
	{
		static constexpr float EPSILON = 3.f;
		bool valid = false;
		if (fabsf(a_posX - m_viewport.x) < EPSILON)
		{
			valid = true;
			a_grabFun = [this](const float a_translation)
				{
					if (m_viewport.width - a_translation > 0)
					{
						m_viewport.x -= a_translation;
						m_viewport.width -= a_translation;
						return true;
					}
					return false;
				};
		}
		else if (fabsf(a_posX - m_viewport.x - m_viewport.width) < EPSILON)
		{
			valid = true;
			a_grabFun = [this](const float a_translation)
				{
					if (m_viewport.width - a_translation > 0)
					{
						m_viewport.width -= a_translation;
						return true;
					}
					return false;
				};
		}
		else if (fabsf(a_posY - m_viewport.y) < EPSILON)
		{
			valid = true;
			a_grabFun = [this](const float a_translation)
				{
					if (m_viewport.height - a_translation > 0)
					{
						m_viewport.y -= a_translation;
						m_viewport.height -= a_translation;
						return true;
					}
					return false;
				};
		}
		else if (fabsf(a_posY - m_viewport.y - m_viewport.height) < EPSILON)
		{
			valid = true;
			a_grabFun = [this](const float a_translation)
				{
					if (m_viewport.height - a_translation > 0)
					{
						m_viewport.height -= a_translation;
						return true;
					}
					return false;
				};
		}
		return valid;
	}

	constexpr [[nodiscard]] bool isIn(const float a_posX, const float a_posY)
	{
		return a_posX >= m_viewport.x && a_posX <= (m_viewport.x + m_viewport.width) &&
			a_posY >= m_viewport.y && a_posY <= (m_viewport.y + m_viewport.height);
	}
};