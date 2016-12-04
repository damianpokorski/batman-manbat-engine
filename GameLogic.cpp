#pragma once
#include "Engine.h"

using namespace std;
using namespace Manbat;

namespace Manbat {
	GameLogic::GameLogic() {
		currentLevel = 0;
		health = 50;
		energy = 75;
		insulin = 80;
		sprinting = false;

		self = ScreenEnum::gameLogic;
		target = ScreenEnum::none;

		DebugMode = false;
		FPSDisplay = false;

		reloadTrigger = false;
		reloadTarget = 1;
	};
	void GameLogic::Init() {
		
		// Time 
		time = 0;
		// Sin power & camera wobble
		sinPower = 0;
		cameraWobble = 0;

		isActive = false;

		//// Loading skybox shaders
		newSkybox = new ImprovedSkybox();
		newSkybox->Init("Shaders/skybox.fx", "Skybox.png");

		// Loading other shaders
		shader = new Effect();
		if (!shader->Load("Shaders/directional_textured_wave.fx")) {
			Debug << "[Shader] Error loading effect file" << std::endl;
		}
		// Load pause screen
		pScreen = new PauseScreen();
		pScreen->isActive = false;
		// Scene
		scene = new Scene();
		scene->Load("TutorialLevel.txt");
		scene->Load("TutorialLevel2.txt");
		// Camera logic
		cam = new ImprovedCamera();
		cam->Init();
		g_engine->addEntity(cam->cameraCollision);
		cam->camera->setPosition(scene->PlayerStart);
		cam->cameraCollision->setPosition(scene->PlayerStart);
		cam->camera->setPerspective(Manbat::PI / 4.0f, (1600.0f / 900.0f) , 1.0f, 10000.0f);
		cam->cameraOffset = cam->cameraOffsetDefault;
		g_engine->playerController = cam;
		// assign camera controller pointer to the enemies, for the state handling purpose
		for (int i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_ENEMY_MESH) {
				((Enemy*)scene->contents[i])->playerController = cam->camera;
			}
		}
		scene->playerController = cam;
		// Load BGM
		
		// Font
		font = new Font("Arial Bold", 30);
		fontSmall = new Font("Arial", 14);
		fontColor = D3DCOLOR_ARGB(128, 255, 255, 255);
		// Insulin Bar Full
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Insulin_Full.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1375, 675);

		// Insulin Empty
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Bar_Empty.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1375, 675);
		Content2D.back()->setSize(200, 100);

		// Energy Bar Full
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Energy_Full.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1225, 675);
		// Energy Empty
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Bar_Empty.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1225, 675);
		Content2D.back()->setSize(200, 100);

		// Health Bar Full
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Health_Full.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1075, 675);

		// Health Empty
		Content2D.push_back(new Sprite());
		Content2D.back()->Load("HUD/UI_Bar_Empty.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(1075, 675);
		Content2D.back()->setSize(200, 100);

		// Floor
		mainplane = new ImprovedMesh();
		mainplane->Init("Scenery/mainplane.x", "Scenery/ground.jpg");
		mainplane->setScale(5);
		mainplane->setCollidable(true);
		mainplane->setAlive(true);
		mainplane->setEntityType(ENTITY_MAINPLANE);
		g_engine->addEntity(mainplane);
		mainplane->Transform();

		g_engine->setGlobalCollisions(true);
	};

	void GameLogic::Update(float deltaTime) {
		// Switching level
		if (reloadTrigger) {
			reloadLevel(reloadTarget);
			reloadTrigger = false;
		}
		// Pause screen
		if (this->pScreen->isActive) {
			pScreen->Update(deltaTime);
			// Parse data acquired from pause state
			if (pScreen->target == ScreenEnum::gameLogic) {
				pScreen->target = ScreenEnum::none;
				pScreen->isActive = false;
				// flying
				cam->flying = pScreen->flying;
				// camera wobble
				if (pScreen->dizziness) {
					sinPower = 0.1;
					animationReset("cameraWobble");
					cameraWobble = 1;
				}
				else {
					sinPower = 0;
					cameraWobble = 0;
				}
				// collision
				g_engine->setGlobalCollisions(pScreen->collision);
				// Show FPS
				FPSDisplay = pScreen->showFPS;
				DebugMode = pScreen->debugMode;
			}
			if (pScreen->target == ScreenEnum::startScreen) {
				pScreen->target = ScreenEnum::none;
				target = ScreenEnum::startScreen;
				pScreen->isActive = false;
			}
			return;
		}
		for (int i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_BILLBOARD){
				if (((Billboard*)scene->contents[i])->AttachedTo == NULL) {
						scene->contents[i]->setPosition(scene->contents[i]->getPosition().getX(),40 + 10 * animationValue("TutorialBillboard", 2.5f, AnimationRepeat::PingPong),scene->contents[i]->getPosition().getZ());
				}
			}
		}
		// Sprinting limited by energy
		if (sprinting && !cam->crouching) {
			if ((energy - (deltaTime * 10)) > 0) {
				cam->characterSpeed = 150;
				energy -= deltaTime * 10;
				// camera "sprint" animation
				cam->cameraOffset = cam->cameraOffsetDefault + Vector3(0.0, 5 * animationValue("cameraSprintAnimation",0.5f, AnimationRepeat::PingPong), 0.0);
			}
			else {
				cam->characterSpeed = 100;
				cam->cameraOffset = cam->cameraOffsetDefault;
			}
		}
		else {
			if (cam->crouching) {
				cam->characterSpeed = 50;
				cam->cameraOffset = cam->cameraOffsetDefault / 2;
			}
			else {
				cam->characterSpeed = 100;
				cam->cameraOffset = cam->cameraOffsetDefault;
			}
		}
		// Handle triggers from player controller
		if (cam->hitByEnemy) {
			health -= 20;
			cam->hitByEnemy = false;
		}
		if (cam->collectableHitEnergy) {
			energy = 100;
			cam->collectableHitEnergy = false;
		}
		if (cam->collectableHitHealth) {
			health = 100;
			cam->collectableHitHealth = false;
		}
		if (cam->collectableHitInsulin) {
			insulin = 100;
			cam->collectableHitInsulin = false;
		}
		// Use insulin to regenerate energy
		if (!sprinting && energy < 80) {
			if (insulin > 0) {
				// Insulin to energy exchange ratio is 1:4
				energy += deltaTime * 20;
				insulin -= deltaTime * 5;
			}
		}
		// Slow health regeneration over time
		if (health < 100 && insulin > 0) {
			health += deltaTime * 3;
		}
		if (cam->jumped) {
			energy -= 20;
			cam->jumped = false;
		}
		cam->canJump = (energy > 20);
		// Player death 
		if (health < 0) {
			health = 50;
			energy = 75;
			insulin = 80;
			cam->camera->setPosition(scene->PlayerStart);
			cam->cameraCollision->setPosition(scene->PlayerStart);
		}
		// Level completion
		if (cam->hitFlag) {
			if (currentLevel == 0) {
				target = Screen::levelSelectionScreen;
				SaveGame::UnlockLevel("One");
				cam->hitFlag = false;
			}
			if (currentLevel == 1) {
				target = Screen::creditsScreen;
				cam->hitFlag = false;
			}
		}
		// Camera "wobble" if player is on low insulin
		if (insulin < 20) {
			cameraWobble = 3;
			sinPower = 0.1f;
		}
		else {
			cameraWobble = 0;
			sinPower = 0;
		}
		if (cameraWobble != 0) {
			cam->camera->setPerspective(Manbat::PI / 4.0f, (1600 / 900) + cameraWobble * animationValue("cameraWobble", 5, AnimationRepeat::PingPong), 1.0f, 10000.0f);
		}
		// Debug mode locks all of the stats in 100% 
		if (DebugMode) {
			health = 100;
			energy = 100;
			insulin = 100;
		}
		// Global time
		time += deltaTime;

		// Animation handling
		// Insulin
		Content2D[1]->setSize(200, 58 + 111 * (1 - (insulin /100)));
		// Energy
		Content2D[3]->setSize(200, 58 + 111 * (1 - (energy / 100)));
		// health
		Content2D[5]->setSize(200, 58 + 111 * (1 -(health / 100)));

		scene->Update(deltaTime);
		//scene->Transform();
		// Updating the camera
		cam->Update(deltaTime);
		// Billboard related
		for (int i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_BILLBOARD) {
				((Billboard*)scene->contents[i])->rotateBillboard(cam->camera);
			}
		}
		// Order billboards by the distance from the camera
		// 3 passes just to make sure it's all sorted out
		if (animationAsTimer("BillboardOrder", 0.5f, true)) {
			scene->sortBillboardsByDistance();
			scene->sortBillboardsByDistance();
			scene->sortBillboardsByDistance();
		}
	}

	void GameLogic::Render3D() {
		// LIGHT STUFF
		shader->setParam("LightColor", D3DXVECTOR4(1, 1, 1, 1));
		shader->setParam("LightVector", D3DXVECTOR3(-1,1,1));
		shader->setParam("LightPower", .5f);
		shader->setParam("Time", time);
		shader->setParam("SinPower", sinPower);

		//newSkybox->RenderSkybox(camera);
		newSkybox->RenderSkybox(cam->camera);

		// Light stuff
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*)&mainplane->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		shader->setParam("WorldInverseTranspose", wit);
		// Light stuff end
		// Render floor then rest of the environment
		mainplane->LazyRender(shader, cam->camera);
		scene->LazyRender(shader, cam->camera);

	}
	void GameLogic::Render2D() {
		if (pScreen->isActive) {
			pScreen->Render2D();
			return;
		}
		for (int i = 0; i < Content2D.size(); i++) {
			Content2D[i]->Render();
		}
		// Bar values
		stringstream ss;
		ss << Math::roundf(health);
		font->Print(1180, 690, ss.str(), fontColor);
		ss.str("");
		ss << Math::roundf(energy);
		font->Print(1330, 690, ss.str(), fontColor);
		ss.str("");
		ss << Math::roundf(insulin);
		font->Print(1480, 690, ss.str(), fontColor);
		// Show FPS
		if (DebugMode) {
			ss.str("");
			ss << "Core Ticks Per Second: " << g_engine->GetCoreFrameRate() << std::endl;
			ss << "Player position" << cam->cameraCollision->getPosition().ToString() << std::endl;
			ss << "Camera rotation" << (cam->camera->getPosition() - cam->camera->p_target).ToString()<< std::endl;
			ss << "Camera position" << cam->camera->getPosition().ToString() << std::endl;
			ss << "Player to floor distance" << Math::Distance(cam->cameraCollision->getPosition(), mainplane->getPosition()) << std::endl;
			ss << "Collision with enemies count:" << cam->enemyCollision << std::endl;
			ss << "Total collideable meshes " << g_engine->p_entities.size() << std::endl;
			ss << "Total meshes within the level" << scene->contents.size() << std::endl;
			font->Print(20, 20, ss.str());
		}
		if (FPSDisplay) {
			ss.str("");
			ss << "FPS: " << g_engine->p_screenFrameRate << std::endl;
			font->Print(1500, 20, ss.str());
		}
	}

	void GameLogic::InputEvent(IEvent* e) {
		if (pScreen->isActive) {
			pScreen->InputEvent(e);
			return;
		}
		cam->InputEvent(e);
		switch (e->GetID()) {
		case EventType::EVENT_KEYPRESS:
			switch (((KeyPressEvent*)e)->keycode) {
				case DIK_LSHIFT:
					if (energy > 0) {
						if (!sprinting) {
							// Reset camera animation to prevent teleporting
							animationReset("cameraSprintAnimation");
						}
						sprinting = true;
					}
					else {
						sprinting = false;
					}
				break;
			}
			break;
		case EventType::EVENT_KEYRELEASE:
			switch (((KeyReleaseEvent*)e)->keycode) {
			case DIK_ESCAPE:
				pScreen->isActive = true;

				pScreen->flying = cam->flying;
				pScreen->dizziness = (sinPower >0);
				pScreen->collision = g_engine->getGlobalCollisions();
				pScreen->debugMode = DebugMode;
				pScreen->showFPS = FPSDisplay;
				pScreen->UpdateOnOff();
				break;
			case DIK_LSHIFT:
				sprinting = false;
			break;
			case DIK_BACKSPACE:
				reloadTrigger = true;
				
				break;
			}
		break;
		case EventType::EVENT_XBUTTON: 
			XButtonEvent* xbe = (XButtonEvent*)e;
			if (xbe->padPointer->IsConnected()) {
				if (xbe->padPointer->buttonReleased(XINPUT_GAMEPAD_START)) {
					pScreen->isActive = true;

					pScreen->flying = cam->flying;
					pScreen->dizziness = (sinPower > 0);
					pScreen->collision = g_engine->getGlobalCollisions();
					pScreen->debugMode = DebugMode;
					pScreen->showFPS = FPSDisplay;
					pScreen->UpdateOnOff();
				}
				if ((xbe->PadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) {
					if (energy > 0) {
						if (!sprinting) {
							// Reset camera animation to prevent teleporting
							animationReset("cameraSprintAnimation");
						}
						sprinting = true;
					}
					else {
						sprinting = false;
					}
				}
				else {
					sprinting = false;
				}
			}
		break;
		}
	}
	void GameLogic::reloadLevel(int levelID) {
		if (currentLevel != levelID) {
			currentLevel = levelID;
			// remove previos scene slements so they dont end up floating in the abyss of RAM
			std::vector<ImprovedMesh*>::iterator iter = scene->contents.begin();
			while (iter != scene->contents.end()) {
					std::vector<Entity*>::iterator iter2 = g_engine->p_entities.begin();
					while (iter2 != g_engine->p_entities.end()) {
						if ((*iter2) == (*iter)) {
							iter2 = g_engine->p_entities.erase(iter2);
						}
						else {
							iter2++;
						}
					}
//					if ((*iter)->materials != NULL) delete[](*iter)->materials;
	//				(*iter)->textures = NULL;
		//			(*iter)->mesh->Release();
					iter = scene->contents.erase(iter);

			}
			// Create new scene
			scene = new Scene();
			switch(levelID){
				case 0:
					scene->Load("TutorialLevel.txt");
					scene->Load("TutorialLevel2.txt");
				break;
				case 1:
					scene->Load("Level.txt");
					scene->Load("Level2.txt");
				break;
			}
		}
		// Camera logic
		
		cam->camera->setPosition(scene->PlayerStart);
		cam->cameraCollision->setPosition(scene->PlayerStart);
		cam->camera->setPerspective(Manbat::PI / 4.0f, (1600.0f / 900.0f), 1.0f, 10000.0f);
		scene->playerController = cam;
		g_engine->playerController = cam;
		cam->hitFlag = false;
		// Assign camera controller pointer to the enemies, for the state handling purpose
		for (int i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_ENEMY_MESH) {
				((Enemy*)scene->contents[i])->playerController = cam->camera;
			}
		}
	}
}