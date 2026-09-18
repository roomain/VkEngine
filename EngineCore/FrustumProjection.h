#pragma once
/***********************************************
* @headerfile FrustumProjection.h
* @date 18 / 09 / 2026
* @author Roomain
************************************************/
#include <limits>
#include <numbers>
#include "ProjectionInterface.h"

/*@brief frustum projection all angles are in radian*/
class FrustumProjection : public ProjectionInterface
{
private:
	glm::dmat4 m_projectionMatrix;	/*!< projection matrix*/

	// projection parameters
	double m_zoom = .0;			/*!< zoom value*/
	double m_fovY = 0.785398;		/*!< Y fov*/
	double m_aspect = 1.0;			/*!< aspect*/
	double m_near = 1.0;
	double m_far = std::numeric_limits<double>::infinity();

public:
	FrustumProjection();
	virtual ~FrustumProjection() final = default;
	explicit FrustumProjection(const double& a_fovY, const double& a_aspect = 1.0, const double a_near = 1.0, const double& a_far = std::numeric_limits<double>::infinity());
	inline [[nodiscard]] glm::dmat4 projectionMat()const noexcept final { return m_projectionMatrix; }
	void setZoom(const double& a_zoom) final;
	inline double zoom()const final { return m_zoom; }
	void viewportResized(const unsigned int a_width, const unsigned int a_height) final;
};