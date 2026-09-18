#include "pch.h"
#include "EngineViewportManager.h"


EngineViewportManager::EngineViewportManager(const unsigned int a_resX, const unsigned int a_resY) : 
	m_resX{ a_resX }, m_resY{ a_resY }
{
	//
}

EngineViewportPtr EngineViewportManager::getViewport(const unsigned int a_posX, const unsigned int a_posY)const
{
	for (auto& pViewport : m_viewports)
	{
		if (pViewport->isIn(static_cast<float>(a_posX), static_cast<float>(a_posY)))
			return pViewport;
	}
	return EngineViewportPtr();
}

void EngineViewportManager::update(const unsigned int a_resX, const unsigned int a_resY)
{
	if (m_resX > 0 && m_resY > 0)
	{
		auto newResX = static_cast<float>(a_resX);
		auto newResY = static_cast<float>(a_resY);

		auto ratioX = newResX / static_cast<float>(m_resX);
		auto ratioY = newResY / static_cast<float>(m_resY);

		for (auto& pViewport : m_viewports)
		{
			pViewport->moveTo(pViewport->x() * ratioX,
				pViewport->y() * ratioY);
			pViewport->resize(pViewport->width() * ratioX,
				pViewport->width() * ratioY);
		}
	}

	m_resX = a_resX;
	m_resY = a_resY;
}

void EngineViewportManager::grab(const float a_posX, const float a_posY, std::vector<ViewportTransform>& a_grab)const
{
	for (auto& pViewport : m_viewports)
		pViewport->grab(a_posX, a_posY, a_grab);
}