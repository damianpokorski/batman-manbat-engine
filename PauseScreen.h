#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {

	class PauseScreen : public Screen
	{
	public:
		bool flying;
		bool dizziness;
		bool collision;
		bool debugMode;
		bool showFPS;
		int CurrentOption;
		PauseScreen() {
			isActive = false;
			self = ScreenEnum::pauseScreen;
			target = ScreenEnum::none;
			CurrentOption = 0;
			time = 0;
			flying = false;
			dizziness = false;
			collision = false;
			debugMode = false;
			showFPS = false;
			//0 Background
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/BG.png")) {
				Debug << "Error creating BG in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(0, 0);
			//1 Flying Text
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/FlyingText.png")) {
				Debug << "Error creating FlyingText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800-133, 400);
			//2 Dizziness Text
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/DizzinessText.png")) {
				Debug << "Error creating DizzinessText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - 201, 440);
			//3 Collision Text
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/CollisionText.png")) {
				Debug << "Error creating CollisionText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - 187, 480);
			//4 DebugMode Text
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/DebugMode.png")) {
				Debug << "Error creating DebugMode in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - 270, 520);

			//5 ShowFPS Text
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/ShowFPSText.png")) {
				Debug << "Error creating ShowFPSText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - 199, 560);

			//6 Return to menu
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/ReturnToMenu.png")) {
				Debug << "Error creating ReturnToMenu in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - (559/2), 650);

			//7 Return to menu
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/Resume.png")) {
				Debug << "Error creating ReturnToMenu in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(800 - (168 / 2), 700);


			//8 Current Option indicator - "square"
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/StartingScreen_Square.png")) {
				Debug << "Error creating StartGame in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(450, 600);
			Content2D.back()->setPivot(Vector2(50, 50));
			Content2D.back()->setScale(0.4);

			//9 Flying Text ON / OFF
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/off.png")) {
				Debug << "Error creating FlyingText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(950, 400);
			//10 Dizziness Text ON / OFF
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/off.png")) {
				Debug << "Error creating DizzinessText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(950, 440);
			//11 Collision Text ON / OFF
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/off.png")) {
				Debug << "Error creating CollisionText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(950, 480);
			//12 DebugMode Text ON / OFF
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/off.png")) {
				Debug << "Error creating DebugMode in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(950, 520);

			//13 ShowFPS Text ON / OFF
			Content2D.push_back(new Sprite());
			if (!Content2D.back()->Load("HUD/Pause/off.png")) {
				Debug << "Error creating ShowFPSText in PauseScreen" << std::endl;
			}
			Content2D.back()->setPosition(950, 560);
		};

		virtual void Init() {

		};
		virtual void Update(float delta) {

			// Options
			time += delta;

			//// Square selection
			Content2D[8]->setPosition(Content2D[CurrentOption + 1]->getPosition().getX()-50, Content2D[CurrentOption + 1]->getPosition().getY());

		};
		virtual void Render2D() {
			for (int i = 0; i < Content2D.size(); i++) {
				Content2D[i]->Render();
			}

		};
		virtual void Render3D() {

		};

		virtual void InputEvent(IEvent * e);
		void UpdateOnOff() {
			if (flying) {
				Content2D[9]->Load("HUD/Pause/on.png");
			}
			else {
				Content2D[9]->Load("HUD/Pause/off.png");
			}
			if (dizziness) {
				Content2D[10]->Load("HUD/Pause/on.png");
			}
			else {
				Content2D[10]->Load("HUD/Pause/off.png");
			}
			if (collision) {
				Content2D[11]->Load("HUD/Pause/on.png");
			}
			else {
				Content2D[11]->Load("HUD/Pause/off.png");
			}
			if (debugMode) {
				Content2D[12]->Load("HUD/Pause/on.png");
			}
			else {
				Content2D[12]->Load("HUD/Pause/off.png");
			}
			if (showFPS) {
				Content2D[13]->Load("HUD/Pause/on.png");
			}
			else {
				Content2D[13]->Load("HUD/Pause/off.png");
			}
		};
	};
};

