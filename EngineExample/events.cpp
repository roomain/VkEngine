#include "events.h"

bool processWindowEvent(const SDL_WindowEvent& a_windoEvent)
{
    //
    return false;
}

bool processKeyboardEvent(const SDL_KeyboardEvent& a_keyboardEvent)
{
    //
    return false;
}

bool processMouseButtonEvent(const SDL_MouseButtonEvent& a_buttonEvent)
{
    //
    return false;
}

bool processMouseMoveEvent(const SDL_MouseMotionEvent& a_buttonEvent)
{
    //
    return false;
}

bool processMouseWheelEvent(const SDL_MouseWheelEvent& a_buttonEvent)
{
    //
    return false;
}

bool processEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_EVENT_QUIT:
		return true;
	case SDL_EVENT_WINDOW_MOVED:             
    case SDL_EVENT_WINDOW_RESIZED:           
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    case SDL_EVENT_WINDOW_MINIMIZED:         
    case SDL_EVENT_WINDOW_MAXIMIZED:         
    case SDL_EVENT_WINDOW_RESTORED:          
    case SDL_EVENT_WINDOW_MOUSE_ENTER:       
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:       
    case SDL_EVENT_WINDOW_FOCUS_GAINED:      
    case SDL_EVENT_WINDOW_FOCUS_LOST:        
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:   
    case SDL_EVENT_WINDOW_HIT_TEST:          
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:   
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:   
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED: 
    case SDL_EVENT_WINDOW_OCCLUDED:          
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:  
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:  
    case SDL_EVENT_WINDOW_DESTROYED:
        return processWindowEvent(event.window);
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        return processKeyboardEvent(event.key);
    case SDL_EVENT_MOUSE_MOTION:
        return processMouseMoveEvent(event.motion);
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        return processMouseButtonEvent(event.button);
    case SDL_EVENT_MOUSE_WHEEL:
        return processMouseWheelEvent(event.wheel);
    default:
        return false;
	}
}