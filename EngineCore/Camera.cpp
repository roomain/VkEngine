#include "pch.h"
#include "Camera.h"
#include "OrthoProjection.h"


Camera::Camera()
{
	m_projection = std::make_shared<OrthoProjection>();
}

Camera::Camera(const ProjectionPtr& a_projection) : m_projection { a_projection }
{
	//
}