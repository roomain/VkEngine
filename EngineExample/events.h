#pragma once
/***********************************************
* @headerfile events.h
* @date 11 / 05 / 2026
* @author Roomain
************************************************/
#include <memory>
#include <SDL3/SDL.h>
#include "EngineRenderer.h"

/*@brief process sdl events return true if quit*/
bool processEvent(const SDL_Event& event, const std::shared_ptr<EngineRenderer>& a_renderer);