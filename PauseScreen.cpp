#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void PauseScreen::InputEvent(IEvent* e) {
		switch (e->GetID()) {
		case EventType::EVENT_KEYRELEASE:
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_DOWN) {
				CurrentOption++;
				if (CurrentOption > 6)
					CurrentOption = 0;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_UP) {
				CurrentOption--;
				if (CurrentOption < 0)
					CurrentOption = 6;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_RETURN) {
				switch (CurrentOption) {
					case 0:
						flying = !flying;
						break;
					case 1:
						dizziness = !dizziness;
					break;
					case 2:
						collision = !collision;
					break;
					case 3:
						debugMode = !debugMode;
					break;
					case 4:
						showFPS = !showFPS;
					break;
					case 5:
						target = ScreenEnum::startScreen;
					break;
					case 6:
						target = ScreenEnum::gameLogic;
					break;
				}
				UpdateOnOff();
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_ESCAPE) {
				target = ScreenEnum::gameLogic;
			}
		break;
		case EventType::EVENT_XBUTTON:
			XButtonEvent* xbe = (XButtonEvent*)e;
			if (xbe->padPointer->IsConnected()) {
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_DOWN)) {
					CurrentOption++;
					if (CurrentOption > 6)
						CurrentOption = 0;
				}
				if(xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_UP)) {
					CurrentOption--;
					if (CurrentOption < 0)
						CurrentOption = 6;
				}
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_START)) {
					target = ScreenEnum::gameLogic;
				}
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_A)) {
					switch (CurrentOption) {
					case 0:
						flying = !flying;
						break;
					case 1:
						dizziness = !dizziness;
						break;
					case 2:
						collision = !collision;
						break;
					case 3:
						debugMode = !debugMode;
						break;
					case 4:
						showFPS = !showFPS;
						break;
					case 5:
						target = ScreenEnum::startScreen;
						break;
					case 6:
						target = ScreenEnum::gameLogic;
						break;
					}
					UpdateOnOff();
				}
			}
		break;
		}
	};
};