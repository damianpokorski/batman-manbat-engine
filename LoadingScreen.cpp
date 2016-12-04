#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void LoadingScreen::InputEvent(IEvent* e) {
		switch (e->GetID()) {
		case EventType::EVENT_KEYRELEASE:
			if ((((KeyReleaseEvent*)e)->keycode) == DIK_RETURN) {
				target = ScreenEnum::levelSelectionScreen;
			}
			break;
		}
	}
}