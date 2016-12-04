#include "Engine.h"

namespace Manbat{
	IEvent::IEvent(){
		id=0;
	}
	KeyPressEvent::KeyPressEvent(int key){
		// This usage of enumerators seems quite weird... I am mighty confused.
		id = EventType::EVENT_KEYPRESS;
		keycode = key;
	}
	KeyReleaseEvent::KeyReleaseEvent(int key){
		id = EventType::EVENT_KEYRELEASE;
		keycode = key;
	}
	MouseClickEvent::MouseClickEvent(int btn){
		id = EventType::EVENT_MOUSECLICK;
		button = btn;
	}
	MouseMotionEvent::MouseMotionEvent(int dx, int dy){
		id = EventType::EVENT_MOUSEMOTION;
		deltax = dx;
		deltay = dy;
	}
	MouseWheelEvent::MouseWheelEvent(int whl){
		id = EventType::EVENT_MOUSEWHEEL;
		wheel = whl;
	}
	MouseMoveEvent::MouseMoveEvent(int px,int py){
		id = EventType::EVENT_MOUSEMOVE;
		posx = px;
		posy = py;
	}
	XButtonEvent::XButtonEvent(XINPUT_STATE padState, XboxPad* padPtr){
		id = EVENT_XBUTTON;
		PadState = padState;
		padPointer = padPtr;
	}
	EntityRenderEvent::EntityRenderEvent(Entity* e){
		id = EVENT_ENTITYRENDER;
		entity = e;
	}
	EntityUpdateEvent::EntityUpdateEvent(Entity* e){
		id = EVENT_ENTITYUPDATE;
		entity = e;
	}
}