#include "pch.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "FrustumProjection.h"

FrustumProjection::FrustumProjection()
{
	m_projectionMatrix = glm::perspective(m_fovY, m_aspect, m_near, m_far);
}

FrustumProjection::FrustumProjection(const double& a_fovY, const double& a_aspect, const double a_near, const double& a_far) :
	m_fovY{ a_fovY }, m_aspect{ a_aspect }, m_near{ a_near }, m_far{ a_far }
{
	m_projectionMatrix = glm::perspective(m_fovY, m_aspect, m_near, m_far);
}

void FrustumProjection::setZoom(const double& a_zoom)
{
	m_zoom = a_zoom;
	m_projectionMatrix = glm::perspective(m_zoom > 0 ? m_fovY / m_zoom : m_fovY, m_aspect, m_near, m_far);
}

void FrustumProjection::viewportResized(const unsigned int a_width, const unsigned int a_height)
{
	m_aspect = static_cast<double>(a_width) / static_cast<double>(a_height);
	m_projectionMatrix = glm::perspective(m_zoom > 0 ? m_fovY / m_zoom : m_fovY, m_aspect, m_near, m_far);
}