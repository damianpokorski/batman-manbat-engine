#pragma once
#include "Engine.h"

namespace Manbat{
	class Input{
	private:
		HWND window;
		IDirectInput8 *di;
		IDirectInputDevice8 *keyboard;
		char keyState[256];
		IDirectInputDevice8 *mouse;
		DIMOUSESTATE mouseState;
		POINT position;
	public:
		Input(HWND window);
		virtual ~Input();
		void Update();
		// getter and setters... yay. feelslikehome.jpg
		char GetKeyState(int key){ 
			return keyState[key];
		};
		long GetMousePosX(){
			return position.x;
		};
		long GetMousePosY(){
			return position.y;
		};
		int GetMouseButton(char button);
		long GetMouseDeltaX() { return mouseState.lX;};
		long GetMouseDeltaY() { return mouseState.lY;};
		long GetMouseDeltaWheel(){ return mouseState.lZ;};
	};
};