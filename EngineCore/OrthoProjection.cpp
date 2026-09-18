#include "pch.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "OrthoProjection.h"


OrthoProjection::OrthoProjection()
{
	m_projectionMatrix = glm::ortho(-1, 1, -1, 1);
}

OrthoProjection::OrthoProjection(const double& a_aspect) : m_aspect{ a_aspect }
{
	const auto semiAspect = m_aspect / 2.0;
	m_projectionMatrix = glm::ortho(-1 * semiAspect, 1 * semiAspect, -1 * semiAspect, 1 * semiAspect);
}

void OrthoProjection::setZoom(const double& a_zoom)
{
	// nothing todo
}

void OrthoProjection::viewportResized(const unsigned int a_width, const unsigned int a_height)
{
	m_aspect = static_cast<double>(a_width) / static_cast<double>(a_height);
	const auto semiAspect = m_aspect / 2.0;
	m_projectionMatrix = glm::ortho(-1 * semiAspect, 1 * semiAspect, -1 * semiAspect, 1 * semiAspect);
}
