#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void CreditsScreen::Update(float delta) {
		// Options
		time += delta;

		//Content2D[1]->setPosition(400, 900.0f - animationValue("LoadingBar", 10, AnimationRepeat::None) * 1000.0f);
		if(animationAsTimer("shutdown", 10, false)){
			g_engine->Shutdown();
		}

	};
	void CreditsScreen::InputEvent(IEvent* e) {

	}
}