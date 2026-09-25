#pragma once
/***********************************************
* @headerfile Entity.h
* @date 15 / 09 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <string>
#include "Transformation.h"
#include "enginecore_globals.h"

class EngineComponent;
using EngineComponentPtr = std::shared_ptr<EngineComponent>;

#pragma warning(push)
#pragma warning( disable : 4251 )

/*@brief Base entity class*/
class ENGINECORE_EXPORT Entity
{
private:
	EngineComponentPtr m_rootComponent;
	Transformation m_transform;

public:
	inline [[nodiscard]] Transformation& transform()
	{
		return m_transform;
	}

	inline [[nodiscard]] EngineComponentPtr rootComponent()const
	{
		return m_rootComponent;
	}

	/*@brief update function called at each frame*/
	/*@param a_time: ms since last frame*/
	virtual void update(const float a_time) = 0;
};
#pragma warning(pop)