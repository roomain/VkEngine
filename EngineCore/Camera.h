#pragma once
/***********************************************
* @headerfile Camera.h
* @date 15 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include "Entity.h"


#pragma warning(push)
#pragma warning( disable : 4251 )

class ProjectionInterface;
using ProjectionPtr = std::shared_ptr<ProjectionInterface>;

class ENGINECORE_EXPORT Camera : public Entity
{
private:
	ProjectionPtr m_projection;

public:
	Camera();
	explicit Camera(const ProjectionPtr& a_projection);
	void setProjection(const ProjectionPtr& a_projection) { m_projection = a_projection; }
	inline [[nodiscard]] ProjectionPtr projection() const noexcept { return m_projection; }
	virtual void update(const float a_time){/*no implementation here*/ }
};

#pragma warning(pop)