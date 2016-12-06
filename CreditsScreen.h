#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {

	class CreditsScreen : public Screen
	{
	public:
		CreditsScreen() {
			isActive = false;
			self = ScreenEnum::creditsScreen;
			target = ScreenEnum::none;
			time = 0;
			//0 Background
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen.png")) {
				Debug << "Error creating BG in CreditsScreen" << std::endl;
			}
			Content2D.back()->setPosition(0, 0);
			// Loading bar BG
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Credits.png")) {
				Debug << "Error creating Credits in CreditsScreen" << std::endl;
			}
			Content2D[1]->setPosition(750, -50);
		};

		virtual void Init() {

		};
		virtual void Update(float delta);
		virtual void Render2D() {
			for (size_t i = 0; i < Content2D.size(); i++) {
				Content2D[i]->Render();
			}
		};
		virtual void Render3D() {

		};

		virtual void InputEvent(IEvent * e);
	};
};

