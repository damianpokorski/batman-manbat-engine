#include "Engine.h"

namespace Manbat{
	Input::Input(HWND hwnd){
		window = hwnd;
		// Create DI 
		DirectInput8Create(
			GetModuleHandle(NULL),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&di,
			NULL
			);
		// Create a keyboard. Whoosh!
		di->CreateDevice(
			GUID_SysKeyboard,
			&keyboard,
			NULL
			);
		keyboard->SetDataFormat(&c_dfDIKeyboard);
		keyboard->SetCooperativeLevel(
			window,
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);
		keyboard->Acquire();
		memset(keyState,0,256);
		// Create a mouse... WHA BAM
		di->CreateDevice(GUID_SysMouse,
			&mouse,
			NULL
			);
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->SetCooperativeLevel(
			window,
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
			);
		mouse->Acquire();
	}
	Input::~Input(){
		// YOU'RE FREE! GOOOOOO
		mouse->Release();
		keyboard->Release();
		di->Release();
	}
	void Input::Update(){
		keyboard->Poll();
		if(!SUCCEEDED(keyboard->GetDeviceState(256,(LPVOID)&keyState))){
			keyboard->Acquire();
		}
		mouse->Poll();
		if(!SUCCEEDED(mouse->GetDeviceState(sizeof(DIMOUSESTATE),&mouseState))){
			mouse->Acquire();
		}

		GetCursorPos(&position);
		ScreenToClient(window,&position);
	}

	int Input::GetMouseButton(char button){
		return (mouseState.rgbButtons[button] & 0x80);
	}
}