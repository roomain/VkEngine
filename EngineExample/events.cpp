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

bool processEvent(const SDL_Event& event, const std::shared_ptr<EngineRenderer>& a_renderer)
{
	switch (event.type)
	{
	case SDL_EVENT_WINDOW_MOVED:
        break;
    case SDL_EVENT_WINDOW_RESIZED:
    {        
       a_renderer->resize(event.window.data1, event.window.data2);
       return false;
    }
        break;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        break;
    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
        break;
    case SDL_EVENT_WINDOW_MINIMIZED:
        break;
    case SDL_EVENT_WINDOW_MAXIMIZED:
        break;
    case SDL_EVENT_WINDOW_RESTORED:
        break;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
        break;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        break;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        break;
    case SDL_EVENT_WINDOW_HIT_TEST:
        break;
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
        break;
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
        break;
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        break;
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
    case SDL_EVENT_QUIT:
        return true;
    default:
        return false;
	}
    return false;
}