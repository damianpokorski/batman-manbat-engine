#pragma once
#include "Engine.h"

namespace Manbat{
	void LevelSelectionScreen::InputEvent(IEvent * e) {
		switch (e->GetID()) {
		case EventType::EVENT_KEYRELEASE:
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_DOWN) {
				CurrentOption++;
				if (CurrentOption > 3)
					CurrentOption = 0;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_UP) {
				CurrentOption--;
				if (CurrentOption < 0)
					CurrentOption = 3;
			}
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_RETURN) {
				if (CurrentOption == 0) {
					target = ScreenEnum::gameLogic;
					selectedLevel = 0;
				}
				if (CurrentOption == 1) {
					if (levelOneAvailable) {
						target = ScreenEnum::gameLogic;
						selectedLevel = 1;
					}
					else {
						nope->Play();
					}
				}
				if (CurrentOption == 2) {
					SaveGame::ResetProgress();
					checkLevelAvailable();
				}
				if (CurrentOption == 3) {
					target = Screen::startScreen;
				}
			}
			break;
		case EventType::EVENT_XBUTTON:
			XButtonEvent* xbe = (XButtonEvent*)e;
			if (xbe->padPointer->IsConnected()) {
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_DOWN)) {
					CurrentOption++;
					if (CurrentOption > 3)
						CurrentOption = 0;
				}
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_DPAD_UP)) {
					CurrentOption--;
					if (CurrentOption < 0)
						CurrentOption = 3;
				}
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_START) || xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_A)) {
					if (CurrentOption == 0) {
						target = ScreenEnum::gameLogic;
						selectedLevel = 0;
					}
					if (CurrentOption == 1) {
						target = ScreenEnum::gameLogic;
						selectedLevel = 1;
					}
					if (CurrentOption == 2) {
						SaveGame::ResetProgress();
					}
					if (CurrentOption == 3) {
						target = Screen::startScreen;
					}
				}
			}
			break;
		}
	}
}