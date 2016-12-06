#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {

	class LoadingScreen : public Screen
	{
	public:
		std::vector<std::string> sillyText;
		Font* font;
		LoadingScreen() {
			isActive = false;
			self = ScreenEnum::loadingScreen;
			target = ScreenEnum::none;
			time = 0;
			font = new Font("Arial Black", 48);
			//0 Background
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen.png")) {
				Debug << "Error creating BG in LoadingScreen" << std::endl;
			}
			Content2D.back()->setPosition(0, 0);
			// Loading bar BG
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Loading/BarOuter.png")) {
				Debug << "Error creating BarOuter in LoadingScreen" << std::endl;
			}
			Content2D.back()->setPosition(200, 600);
			// Loading bar front
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Loading/BarInner.png")) {
				Debug << "Error creating BarInner in LoadingScreen" << std::endl;
			}
			Content2D.back()->setPosition(200, 600);
			// add silly text
			sillyText.push_back("Summoning enemies...");
			sillyText.push_back("Firing proton torpedoes..");
			sillyText.push_back("Painting walls...");
			sillyText.push_back("Acquiring vast amounts of cocaine...");
			sillyText.push_back("Solving first world problems...");
			sillyText.push_back("Finding the dankest of memes...");
			sillyText.push_back("Looking for rarest of pepes...");
			sillyText.push_back("Filling up potion bottles...");
			sillyText.push_back("Dusting off the magic carpets...");
			sillyText.push_back("Running around at the speed of sound...");
			sillyText.push_back("Downloading more RAM...");
			sillyText.push_back("Carefully placing boxes..");
			sillyText.push_back("Stumping the Trump..");
			std::random_shuffle(sillyText.begin(), sillyText.end());
		};

		virtual void Init() {

		};
		virtual void Update(float delta) {
			// Options
			time += delta;
			
			Content2D[2]->setSize((int)(animationValue("LoadingBar", 5, AnimationRepeat::PingPong) * 1200), 50);
			if(animationAsTimer("LoadingScreen", 3)) {
				animationReset("LoadingScreen");
			}
		};
		virtual void Render2D() {
			for (size_t i = 0; i < Content2D.size(); i++) {
				Content2D[i]->Render();
			}
			font->Print(400, 550, sillyText[0]);

			if (animationAsTimer("LoadingScreenAnim", 0.5f)) {
				std::random_shuffle(sillyText.begin(), sillyText.end());
			}
			if (animationAsTimer("LoadingTimer", 5)) {
				this->target = ScreenEnum::levelSelectionScreen;
			}
		};
		virtual void Render3D() {

		};

		virtual void InputEvent(IEvent * e);
	};
};

