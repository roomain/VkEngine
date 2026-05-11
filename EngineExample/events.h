#pragma once
/***********************************************
* @headerfile events.h
* @date 11 / 05 / 2026
* @author Roomain
************************************************/
#include <SDL3/SDL.h>

/*@brief process sdl events return true if quit*/
bool processEvent(const SDL_Event& event);