#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void StartScreen::InputEvent(IEvent* e) {
		switch (e->GetID()) {
		case EventType::EVENT_KEYRELEASE:
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_DOWN) {
				CurrentOption++;
				if (CurrentOption > 1)
					CurrentOption = 0;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_UP) {
				CurrentOption++;
				if (CurrentOption > 1)
					CurrentOption = 0;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_RETURN) {
				if (CurrentOption == 0) {
					target = ScreenEnum::loadingScreen;
				}
				if (CurrentOption == 1) {
					g_engine->Shutdown();
				}
			}
		break;
		case EventType::EVENT_XBUTTON:
			XButtonEvent* xbe = (XButtonEvent*)e;
			if (xbe->padPointer->IsConnected()) {
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_DOWN)) {
					CurrentOption++;
					if (CurrentOption > 1)
						CurrentOption = 0;
				}
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_UP)) {
					CurrentOption++;
					if (CurrentOption > 1)
						CurrentOption = 0;
				}
				if(xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_START) || xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_A)){
					if (CurrentOption == 0) {
						target = ScreenEnum::loadingScreen;
					}
					if (CurrentOption == 1) {
						g_engine->Shutdown();
					}
				}
			}
		break;
		}
	};
};