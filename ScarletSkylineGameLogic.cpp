#pragma once
#include "Engine.h"
#include "ScarletSkylineGameLogic.h"

using namespace std;
using namespace Manbat;

namespace Manbat {
	ScarletSkylineGameLogic::ScarletSkylineGameLogic() {
		currentLevel = 0;
		sprinting = false;

		self = ScreenEnum::gameLogic;
		target = ScreenEnum::none;

		DebugMode = false;
		FPSDisplay = false;

		reloadTrigger = false;
		reloadTarget = 1;
	};
	void ScarletSkylineGameLogic::Init() {

		// Time 
		time = 0;
		// Sin power & camera wobble
		sinPower = 0;
		cameraWobble = 0;

		isActive = false;

		//// Loading skybox shaders
		newSkybox = new ImprovedSkybox();
		newSkybox->Init("Shaders/skybox.fx", "mtns.jpg");

		// Loading other shaders
		shader = new Effect();
		if (!shader->Load("Shaders/directional_textured_wave.fx")) {
			Debug << "[Shader] Error loading effect file" << std::endl;
		}
		// Load pause screen
		pScreen = new PauseScreen();
		// Scale down pause screen from insulin rush resolution to scarlet skylines resolution
		for(auto element: pScreen->Content2D)
		{
			element->setX(element->getX() - 290);
			element->setY(element->getY() - 190);
			//element->setScale(0.2);
		}
		//
		pScreen->isActive = false;
		// Scene
		scene = new Scene();
		scene->Load("ScarletLevel.txt");
		// Camera logic
		cam = new ImprovedCamera();
		cam->Init();
		g_engine->addEntity(cam->cameraCollision);
		cam->camera->setPosition(scene->PlayerStart);
		cam->cameraCollision->setPosition(scene->PlayerStart);
		cam->camera->setPerspective(Manbat::PI / 4.0f, (1600.0f / 900.0f), 1.0f, 10000.0f);
		cam->cameraOffset = cam->cameraOffsetDefault;
		g_engine->playerController = cam;
		// assign camera controller pointer to the enemies, for the state handling purpose
		for (size_t i = 0; i < scene->contents.size(); i++) {
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
		// Floor
		mainplane = new ImprovedMesh();
		mainplane->Init("Scenery/mainplane.x", "Scenery/ground.jpg");
		mainplane->setScale(5);
		mainplane->setCollidable(true);
		mainplane->setAlive(true);
		mainplane->setEntityType(ENTITY_MAINPLANE);
		g_engine->addEntity(mainplane);
		mainplane->Transform();
		// Insulin Empty
		this->MiniMap = new Sprite();
		Content2D.push_back(MiniMap);
		Content2D.back()->Load("minimap/mapbg.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(750, 500);
		Content2D.back()->setSize(200, 200);
		this->MiniMapPlayer = new Sprite();
		Content2D.push_back(MiniMapPlayer);
		Content2D.back()->Load("minimap/charmap.png");
		Content2D.back()->setPivot(Vector2(0, 0));
		Content2D.back()->setPosition(750, 500);
		Content2D.back()->setSize(10, 10);
		//
		g_engine->setGlobalCollisions(true);
	};

	void ScarletSkylineGameLogic::Update(float deltaTime) {
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
		for (size_t i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_BILLBOARD) {
				if (((Billboard*)scene->contents[i])->AttachedTo == NULL) {
					scene->contents[i]->setPosition(scene->contents[i]->getPosition().getX(), 40 + 10 * animationValue("TutorialBillboard", 2.5f, AnimationRepeat::PingPong), scene->contents[i]->getPosition().getZ());
				}
			}
		}
		// Sprinting limited by energy
		if (sprinting && !cam->crouching) {
				cam->characterSpeed = 150;
				// camera "sprint" animation
				cam->cameraOffset = cam->cameraOffsetDefault + Vector3(0.0, 5 * animationValue("cameraSprintAnimation", 0.5f, AnimationRepeat::PingPong), 0.0);
		}
		else{
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
			//health -= 20;
			cam->hitByEnemy = false;
		}
		if (cam->collectableHitEnergy) {
			cam->collectableHitEnergy = false;
		}
		if (cam->collectableHitHealth) {
			cam->collectableHitHealth = false;
		}
		if (cam->collectableHitInsulin) {
			cam->collectableHitInsulin = false;
		}

		// Slow health regeneration over time
		
		cam->canJump = true;
		cameraWobble = 0;
		sinPower = 0;
		// Global time
		time += deltaTime;

		scene->Update(deltaTime);
		//scene->Transform();
		// Updating the camera
		cam->Update(deltaTime);
		// Billboard related
		for (size_t i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_BILLBOARD) {
				((Billboard*)scene->contents[i])->rotateBillboard(cam->camera);
			}
		}
		// Draw content to minimap
		// Order billboards by the distance from the camera
		// 3 passes just to make sure it's all sorted out
		if (animationAsTimer("BillboardOrder", 0.5f, true)) {
			scene->sortBillboardsByDistance();
			scene->sortBillboardsByDistance();
			scene->sortBillboardsByDistance();
		}
	}

	void ScarletSkylineGameLogic::Render3D() {
		// LIGHT STUFF
		shader->setParam("LightColor", D3DXVECTOR4(1, 1, 1, 1));
		shader->setParam("LightVector", D3DXVECTOR3(-1, 1, 1));
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
	void ScarletSkylineGameLogic::Render2D() {
		// Update players position on the minimap
		this->MiniMapPlayer->setPosition(ObjectToMap(cam->camera->getPosition()));
		if (pScreen->isActive) {
			pScreen->Render2D();
			return;
		}
		for (size_t i = 0; i < Content2D.size(); i++) {
			Content2D[i]->Render();
		}
		// Bar values
		stringstream ss;
		// Show FPS
		if (DebugMode) {
			ss.str("");
			ss << "Core Ticks Per Second: " << g_engine->GetCoreFrameRate() << std::endl;
			ss << "Player position" << cam->cameraCollision->getPosition().ToString() << std::endl;
			ss << "Camera rotation" << (cam->camera->getPosition() - cam->camera->p_target).ToString() << std::endl;
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
			font->Print(950, 20, ss.str());
		}
	}

	void ScarletSkylineGameLogic::InputEvent(IEvent* e) {
		if (pScreen->isActive) {
			pScreen->InputEvent(e);
			return;
		}
		cam->InputEvent(e);
		switch (e->GetID()) {
		case EventType::EVENT_KEYPRESS:
			switch (((KeyPressEvent*)e)->keycode) {
			case DIK_LSHIFT:
				if (!sprinting) {
					// Reset camera animation to prevent teleporting
					animationReset("cameraSprintAnimation");
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
			break;
		}
	}
	void ScarletSkylineGameLogic::reloadLevel(int levelID) {
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
				//			if ((*iter)->materials != NULL) delete[](*iter)->materials;
				//				(*iter)->textures = NULL;
				//			(*iter)->mesh->Release();

				iter = scene->contents.erase(iter);
			}
			// Remove all of the entities
			g_engine->p_entities.clear();
			g_engine->SortedEntities.clear();
			//// Reinitialize the floor
			//mainplane = new ImprovedMesh();
			//mainplane->Init("Scenery/mainplane.x", "Scenery/ground.jpg");
			//mainplane->setScale(5);
			//mainplane->setCollidable(true);
			//mainplane->setAlive(true);
			//mainplane->setEntityType(ENTITY_MAINPLANE);
			//g_engine->addEntity(mainplane);
			//mainplane->Transform();

			// Create new scene
			scene = new Scene();
			switch (levelID) {
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
		for (size_t i = 0; i < scene->contents.size(); i++) {
			if (scene->contents[i]->getEntityType() == ENTITY_ENEMY_MESH) {
				((Enemy*)scene->contents[i])->playerController = cam->camera;
			}
		}
	}
}