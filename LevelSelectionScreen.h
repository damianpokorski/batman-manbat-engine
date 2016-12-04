#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {

	class LevelSelectionScreen : public Screen
	{
	public:

		int CurrentOption;
		int selectedLevel;
		bool levelOneAvailable;
		cSound* nope;
		void checkLevelAvailable() {
			levelOneAvailable = SaveGame::IsLevelUnlocked("One");
			if (levelOneAvailable) {
				Content2D[2]->Load("HUD/LevelFirst.png");
			}
			else {
				Content2D[2]->Load("HUD/LevelFirstLocked.png");
			}
		};
		LevelSelectionScreen() {
			isActive = false;
			self = ScreenEnum::levelSelectionScreen;
			target = ScreenEnum::none;
			CurrentOption = 0;
			selectedLevel = 0;
			time = 0;
			// 0 - Background
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen.png")) {
				Debug << "Error creating BG in LevelSelectionScreen" << std::endl;
			}
			Content2D.back()->setPosition(0, 0);
			// 1 - Tutorial level
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/LevelTutorial.png")) {
				Debug << "Error creating LevelTutorial in LevelSelectionScreen" << std::endl;
			}
			Content2D.back()->setPosition(750, 350);
			// 2 - Second level
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/LevelFirst.png")) {
				Debug << "Error creating LevelFirst in LevelSelectionScreen" << std::endl;
			}
			Content2D.back()->setPosition(750, 425);
			// 3 - Reset Progress
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/ResetProgress.png")) {
				Debug << "Error creating ResetProgress in LevelSelectionScreen" << std::endl;
			}
			Content2D.back()->setPosition(750, 500);
			// 4 - Return to menu
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Back.png")) {
				Debug << "Error creating Back in LevelSelectionScreen" << std::endl;
			}
			Content2D.back()->setPosition(750, 575);
			// 5 - Current Option indicator - "square"
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen_Square.png")) {
				Debug << "Error creating StartingScreen_Square in LevelSelectionScreen " << std::endl;
			}
			Content2D.back()->setPosition(700, 650);
			Content2D.back()->setPivot(Vector2(50, 50));
			checkLevelAvailable();
			nope = new cSound("Sound/Chris.wav");
		};

		virtual void Init() {

		};

		virtual void Update(float delta) {
			// Animation
			time += delta;
			// More animation stuff
			Content2D[1]->setPosition(750 + animationValue("LevelSelection", 2, Screen::PingPong) * 25, Content2D[1]->getPosition().getY());
			Content2D[2]->setPosition(750 + animationValue("LevelSelection", 2, Screen::PingPong) * 50, Content2D[2]->getPosition().getY());
			Content2D[3]->setPosition(750 + animationValue("LevelSelection", 2, Screen::PingPong) * 75, Content2D[3]->getPosition().getY());
			Content2D[4]->setPosition(750 + animationValue("LevelSelection", 2, Screen::PingPong) * 100, Content2D[4]->getPosition().getY());
			// Square selection
			Content2D[5]->setPosition(Content2D[CurrentOption + 1]->getPosition().getX() - 125, Content2D[CurrentOption + 1]->getPosition().getY() - 25);
			Content2D[5]->setRotation(Content2D[5]->getRotation() + delta * 5);
			// check if level was unlocked every one second or so
			if (animationAsTimer("levelUnlockedCheck", 1.0f, true)) {
				checkLevelAvailable();
			}
		};
		virtual void Render2D() {
			for (int i = 0; i < Content2D.size(); i++) {
				Content2D[i]->Render();
			}

		};
		virtual void Render3D() {

		};

		virtual void InputEvent(IEvent * e);

	};
};
