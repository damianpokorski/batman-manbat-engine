#include "Engine.h"

using namespace std;
using namespace Manbat;


std::vector<Screen*> GameStates;
cSound* BGM;
bool game_preload() {
	
	if (g_engine->p_commandLineParams == "insulin")
	{
		//Insulin Rush
		g_engine->SetAppTitle("insulin");
		g_engine->SetScreen(1600, 900, 32, false);
	}else
	{
		//Scarlet Skyline
		g_engine->SetAppTitle("scarlet_skyline");
		g_engine->SetScreen(1024, 720, 32, false);
	}
	
	 
	 //Scarlet Skyline
	 
	 
	 
	return true;
}

bool game_init(HWND hwnd) {
	// Initialize sound
	cSoundLayer::Create(g_engine->GetWindowHandle());
	BGM = new cSound("Sound/BGM.wav");
	BGM->Play(true);
	// Decide which game is being played
	if (g_engine->p_commandLineParams == "insulin")
	{
		//Insulin Rush
		GameStates.push_back(new StartScreen());
		GameStates.back()->isActive = true;
		GameStates.push_back(new LevelSelectionScreen());
		GameStates.push_back(new LoadingScreen());
		GameStates.push_back(new GameLogic());
		GameStates.back()->Init();
		GameStates.push_back(new CreditsScreen());
	}
	else
	{
		//Scarlet skyline
		GameStates.push_back(new CharacterLoader());
		GameStates.push_back(new ScarletSkylineGameLogic());
		GameStates.back()->Init();
		GameStates.back()->isActive = true;
		
	}
	return true;
}

void game_end() {
	g_engine->buryEntities();
	for (size_t i = 0; i < GameStates.size(); i++) {
		delete GameStates[i];
	}
}

void game_update(float deltaTime) {
	Manbat::Timer T;
	// If see if any gamestate calls for a change
	for (size_t i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->target != Screen::ScreenEnum::none) {
			// Pass selected level from selection screen, I know it's an ugly solution.. but well.. it's an exception.
			if (GameStates[i]->self == Screen::levelSelectionScreen && GameStates[i]->target == Screen::gameLogic) {
				for (size_t j = 0; j < GameStates.size(); j++) {
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
				for (size_t j = 0; j < GameStates.size(); j++) {
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
	for (size_t i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Update(deltaTime);
		}
	}
	//Debug << "[Engine][Update] Update took 1/" << (1.0f / T.getElapsedClock()) << std::endl;
}

void game_render3D() {
	for (size_t i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Render3D();
		}
	}
}

void game_render2D() {
	for (size_t i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->Render2D();
		}
	}
}

void game_event(IEvent* e) {
	for (size_t i = 0; i < GameStates.size(); i++) {
		if (GameStates[i]->isActive) {
			GameStates[i]->InputEvent(e);
		}
	}
}