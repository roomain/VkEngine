#pragma once
/***********************************************
* @headerfile OrthoProjection.h
* @date 18 / 09 / 2026
* @author Roomain
************************************************/
#include "ProjectionInterface.h"

/*@brief ortho projection*/
class OrthoProjection : public ProjectionInterface
{
private:
	glm::dmat4 m_projectionMatrix;	/*!< projection matrix*/

	// projection parameters
	double m_zoom = 1.0;
	double m_aspect = 1.0;

public:
	OrthoProjection();
	virtual ~OrthoProjection() final = default;
	explicit OrthoProjection(const double& a_aspect);
	inline [[nodiscard]] glm::dmat4 projectionMat()const noexcept final { return m_projectionMatrix; }
	void setZoom(const double& a_zoom) final;
	inline double zoom()const final { return 1.0; }
	void viewportResized(const unsigned int a_width, const unsigned int a_height) final;
};
