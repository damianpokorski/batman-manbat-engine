#pragma once

namespace Manbat{
	class IEvent{
		protected:
			int id;
		public:
			IEvent();
			virtual ~IEvent(){};
			int GetID() {
				return id; 
			};
	};
	// Whats up with the incrementations by 10? >_>
	enum EventType{
		EVENT_TIMER = 10,
		EVENT_KEYPRESS = 20,
		EVENT_KEYRELEASE = 30,
		EVENT_MOUSECLICK = 40,
		EVENT_MOUSEMOTION = 50,
		EVENT_MOUSEWHEEL = 60,
		EVENT_MOUSEMOVE = 70,
		EVENT_XBUTTON = 80,
		EVENT_ENTITYUPDATE = 90,
		EVENT_ENTITYRENDER = 100
	};
	class KeyPressEvent: public IEvent{
		public:
			int keycode;
			KeyPressEvent(int key);
	};
	class KeyReleaseEvent: public IEvent{
		public:
			int keycode;
			KeyReleaseEvent(int key);
	}; 
	class MouseClickEvent: public IEvent{
		public:
			int button;
			MouseClickEvent(int btn);
	};
	class MouseMotionEvent: public IEvent{
		public:
			int deltax,deltay;
			MouseMotionEvent(int dx,int dy);
	};
	class MouseWheelEvent: public IEvent{
		public:
			int wheel;
			MouseWheelEvent(int wheel);
	};
	class MouseMoveEvent: public IEvent{
	public:
		int posx,posy;
		MouseMoveEvent(int px,int py);
	};
	class XButtonEvent : public IEvent{
	public:
		XINPUT_STATE PadState;
		XboxPad* padPointer;
		XButtonEvent(XINPUT_STATE padState, XboxPad* padPtr);
	};
	class EntityRenderEvent : public IEvent{
		public:
			Manbat::Entity* entity;
			EntityRenderEvent(Entity* e);
	};
	class EntityUpdateEvent : public IEvent{
	public:
		Manbat::Entity* entity;
		EntityUpdateEvent(Entity* e);
	};
};