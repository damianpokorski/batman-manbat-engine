#include "Engine.h"

using namespace std;
using namespace Manbat;


std::vector<Screen*> GameStates;
cSound* BGM;
bool game_preload() {
	g_engine->SetAppTitle("Insulin rush");
	g_engine->SetScreen(1600, 900, 32, false);

	return true;
}

bool game_init(HWND hwnd) {
	// Initialize sound
	cSoundLayer::Create(g_engine->GetWindowHandle());
	BGM = new cSound("Sound/BGM.wav");
	BGM->Play(true);
	
	GameStates.push_back(new StartScreen());
	GameStates.back()->isActive = true;
	GameStates.push_back(new LevelSelectionScreen());
	GameStates.push_back(new LoadingScreen());
	GameStates.push_back(new GameLogic());
	GameStates.back()->Init();
	GameStates.push_back(new CreditsScreen());
	//GameStates.back()->isActive = true;
	
	return true;
}

void game_end() {
	g_engine->buryEntities();
	for (int i = 0; i < GameStates.size(); i++) {
		delete GameStates[i];
	}
}

void game_update(float deltaTime) {
	// If see if any gamestate calls for a change
	for (int i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->target != Screen::ScreenEnum::none) {
			// Pass selected level from selection screen, I know it's an ugly solution.. but well.. it's an exception.
			if (GameStates[i]->self == Screen::levelSelectionScreen && GameStates[i]->target == Screen::gameLogic) {
				for (int j = 0; j < GameStates.size(); j++) {
					if (GameStates[j]->self == GameStates[i]->target) {
						((GameLogic*)GameStates[j])->reloadTarget = ((LevelSelectionScreen*)GameStates[i])->selectedLevel;
						((GameLogic*)GameStates[j])->reloadTrigger = true;
						GameStates[j]->isActive = true;
					}
					else {
						GameStates[j]->isActive = false;
					}
				}
			}
			else {
				for (int j = 0; j < GameStates.size(); j++) {
					if (GameStates[j]->self == GameStates[i]->target) {
						GameStates[j]->isActive = true;
					}
					else {
						GameStates[j]->isActive = false;
					}
				}
			}
			GameStates[i]->target = Screen::ScreenEnum::none;
		}
	}
	for (int i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Update(deltaTime);
		}
	}
}

void game_render3D() {
	for (int i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Render3D();
		}
	}
}

void game_render2D() {
	for (int i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Render2D();
		}
	}
}

void game_event(IEvent* e) {
	for (int i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->InputEvent(e);
		}
	}
}