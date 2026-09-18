#pragma once
/***********************************************
* @headerfile ProjectionInterface.h
* @date 17 / 09 / 2026
* @author Roomain
************************************************/
#include <glm/glm.hpp>

/*@brief interface for projection*/
class ProjectionInterface
{		
public:
	virtual ~ProjectionInterface() = default;
	virtual [[nodiscard]] glm::dmat4 projectionMat()const noexcept = 0;
	virtual void setZoom(const double& a_zoom) = 0;
	virtual double zoom()const = 0;
	virtual void viewportResized(const unsigned int a_width, const unsigned int a_height) = 0;
};
