#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	Scene::Scene() {
		Debug << "[Scene] Scene loader initiated." << std::endl;
		playerController = NULL;
	}

	void Scene::Load(char* filename) {

		//
		int x = 0;
		int z = 0;

		// Read file line by line
		std::string line;
		std::ifstream level(filename);
		if (level.is_open())
		{
			while (level.good())
			{
				getline(level, line);
				Debug << "[Scene] " << line << std::endl;
				x = 0;
				for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
					Add((*it), x, z);
					x++;
				}
				z++;
			}
			level.close();
		}
		else
		{
			Debug << "[Scene] Unable to open file" << std::endl << std::endl;
		}
	}

	void Scene::Add(char Element, int x, int z) {
		int multiplier = 50;
		// C - Carrot & A billboard representing state of it
		// H - Health pickup
		// E - Energy pickup
		// F - Finish / Flag
		// I - Insulin pickup
		// K - Kermit
		// X - Box
		// - - Jumpable obstacle
		// P - Player starting position
		// 1-9 Tutorial billboards
		// Carrot
		if (Element == 'C') {
			Enemy* temp = new Enemy();
			temp->Init("Scenery/carrot9.x", "Scenery/carrot.png");
			temp->setPosition(x*multiplier, 5, z*multiplier*-1);
			temp->setScale(10.0f);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setRotation(0, 0, 0);
			temp->setEntityType(ENTITY_ENEMY_MESH);
			contents.push_back(temp);
			g_engine->addEntity(temp);

			Billboard* temp2 = new Billboard();
			temp2->Init("Scenery/maplane.x", "HUD/StateIndicators/Roam.png");
			temp2->setPosition((-multiplier / 2) + x*multiplier, 80, (multiplier / 2) + z*multiplier*-1);
			temp2->setScale(5,2.5,5);
			temp2->setCollidable(false);
			temp2->setAlive(true);
			temp2->setRotation(0, -90, 90);
			temp2->setEntityType(ENTITY_BILLBOARD);
			temp2->AttachedTo = temp;
			temp->stateIndicator = temp2;
			contents.push_back(temp2);
			g_engine->addEntity(temp2);
		}
		// Energy
		if (Element == 'E') {
			ImprovedCollectable* temp = new ImprovedCollectable();
			temp->Init("Scenery/crate.x", "Collectables/EnergyBox.png");
			temp->setPosition(x*multiplier, multiplier / 2, z*multiplier*-1);
			temp->setScale(0.25);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_IMPROVED_COLLECTABLE_MESH);
			temp->collectableType = ImprovedCollectable::CollectableType::Energy;
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		if (Element == 'F') {
			ImprovedMesh* temp = new ImprovedMesh();
			temp->Init("Scenery/crate.x", "Scenery/flagPole.png");
			temp->setPosition(x*multiplier, 75, z*multiplier*-1);
			temp->setScale(.1, 3, .1);
			temp->setCollidable(false);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
			temp = new ImprovedMesh();
			temp->Init("Scenery/crate.x", "Scenery/flag.png");
			temp->setPosition(x*multiplier, 125,5+ z*multiplier*-1);
			temp->setScale(.05, 1, 1.5);
			temp->setCollidable(false);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
			temp = new ImprovedMesh();
			temp->Init("Scenery/crate.x", "Scenery/portal2.jpg");
			temp->setPosition(x*multiplier, 25, z*multiplier*-1);
			temp->setScale(1, 1, 1);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_FLAG);
			g_engine->addEntity(temp);
		}
		// Health
		if (Element == 'H') {
			ImprovedCollectable* temp = new ImprovedCollectable();
			temp->Init("Scenery/crate.x", "Collectables/HealthBox.png");
			temp->setPosition(x*multiplier, multiplier / 2, z*multiplier*-1);
			temp->setScale(0.25);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_IMPROVED_COLLECTABLE_MESH);
			temp->collectableType = ImprovedCollectable::CollectableType::Health;
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		// Insulin
		if (Element == 'I') {
			ImprovedCollectable* temp = new ImprovedCollectable();
			temp->Init("Scenery/crate.x", "Collectables/InsulinBox.png");
			temp->setPosition(x*multiplier, multiplier / 2, z*multiplier*-1);
			temp->setScale(0.25);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_IMPROVED_COLLECTABLE_MESH);
			temp->collectableType = ImprovedCollectable::CollectableType::Insulin;
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		// Kermit!
		if (Element == 'K') {
			AnimatedBillboard* temp = new AnimatedBillboard();
			temp->Init("Scenery/maplane.x", "Scenery/Billboard/sample.png");
			// Positioning billboards to the center of a "block" as they rotate around center
			temp->setPosition((-multiplier/2) + x*multiplier, 50,(multiplier/2)+ z*multiplier*-1);
			temp->setScale(25* 1.77777777778, 25* 0.5625, 25);
			temp->setCollidable(false);
			temp->setAlive(true);
			temp->setRotation(0, -90, 90);
			temp->setEntityType(ENTITY_BILLBOARD);
			temp->LoadTextures("kermit/",".png", 0, 10);
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		// Enemy Skelibobs
		if (Element == 'L') {
			Enemy* temp = new Enemy();
			temp->Init("bones_all.x", "bones.bmp");
			temp->setPosition(x*multiplier, 5, z*multiplier*-1);
			temp->setScale(25.0f);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setRotation(0, 0, 0);
			temp->setEntityType(ENTITY_ENEMY_MESH);
			contents.push_back(temp);
			g_engine->addEntity(temp);

		}
		// Boxes
		if (Element == 'X') {
			ImprovedMesh* temp = new ImprovedMesh();
			//Enemy* temp = new Enemy();
			temp->Init("Scenery/crate.x", "Scenery/portal2.jpg");
			temp->setPosition(x*multiplier, 25, z*multiplier*-1);
			temp->setScale(1,1,1);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		if (Element == 'Y') {
			ImprovedMesh* temp = new ImprovedMesh();
			//Enemy* temp = new Enemy();
			temp->Init("Scenery/crate.x", "Scenery/portal2.jpg");
			temp->setPosition(x*multiplier, 75, z*multiplier*-1);
			temp->setScale(1, 1, 1);
			temp->setCollidable(false);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
		}
		if (Element == 'Z') {
			ImprovedMesh* temp = new ImprovedMesh();
			//Enemy* temp = new Enemy();
			temp->Init("Scenery/crate.x", "Scenery/portal2.jpg");
			temp->setPosition(x*multiplier, 75, z*multiplier*-1);
			temp->setScale(1, 1, 1);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
			//g_engine->addEntity(temp);
		}
		// Jumpable obstacles
		if (Element == '-') {
			ImprovedMesh* temp = new ImprovedMesh();
			//Enemy* temp = new Enemy();
			temp->Init("Scenery/crate.x", "Scenery/portal2.jpg");
			temp->setPosition(x*multiplier,0, z*multiplier*-1);
			temp->setScale(1);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_SCENERY_MESH);
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		if (Element == 'P') {
			PlayerStart = Vector3(x*multiplier, 50, z*multiplier*-1);
		}
		// Scarlet Skyline pickups
		if (Element == '!' ||	// Shotgun
			Element == '"' ||	// Revolver
			Element == '£' ||	// TwoByFour
			Element == '$' ||	// Molotov
			Element == '%' ||	// TNT
			Element == '^' ||	// SHELLS
			Element == '&' ||	// BULLETS
			Element == '*' ||	// BANDAGES
			Element == '(' ||   // JAHNWAYNE
			Element == ')' 	// JAHNWAYNE
			) {
			ImprovedCollectable* temp = new ImprovedCollectable();
			
			switch(Element)
			{
				case '!':
					temp->Init("Scenery/crate.x", "Collectables/shotup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::Shotgun;
					break;
				case '"':
					temp->Init("Scenery/crate.x", "Collectables/revup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::Revolver;
					break;
				case '£':
					temp->Init("Scenery/crate.x", "Collectables/bandup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::TwoByFour;
					break;
				case '$':
					temp->Init("Scenery/crate.x", "Collectables/moltup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::Molotov;
					break;
				case '%':
					temp->Init("Scenery/crate.x", "Collectables/tntup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::TNT;
					break;
				case '^':
					temp->Init("Scenery/crate.x", "Collectables/shellup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::SHELLS;
					break;
				case '&':
					temp->Init("Scenery/crate.x", "Collectables/bullup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::BULLETS;
					break;
				case '*':
					temp->Init("Scenery/crate.x", "Collectables/bandup.png");
					temp->collectableType = ImprovedCollectable::CollectableType::BANDAGES;
					break;
				case '(':
					temp->Init("Scenery/crate.x", "Collectables/john_up.png");
					temp->collectableType = ImprovedCollectable::CollectableType::JAHNWAYNE;
					break;
				case ')':
					temp->Init("Scenery/crate.x", "Collectables/cash.png");
					temp->collectableType = ImprovedCollectable::CollectableType::Cash;
					break;
				default: break;
			}
			temp->setPosition(x*multiplier, multiplier / 2, z*multiplier*-1);
			temp->setScale(0.25);
			temp->setCollidable(true);
			temp->setAlive(true);
			temp->setEntityType(ENTITY_IMPROVED_COLLECTABLE_MESH);
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
			
		// Tutorial billboards
		if (
			Element == '0' ||
			Element == '1' ||
			Element == '2' ||
			Element == '3' ||
			Element == '4' ||
			Element == '5' ||
			Element == '6' ||
			Element == '7' ||
			Element == '8' ||
			Element == '9'
			) {
			Billboard* temp = new Billboard();
			if (Element == '0') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Camera.png");}
			if (Element == '1') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Walking.png"); }
			if (Element == '2') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Jump.png"); }
			if (Element == '3') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Sprint.png"); }
			if (Element == '4') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/EnergyCost.png"); }
			if (Element == '5') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Health.png"); }
			if (Element == '6') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Energy.png"); }
			if (Element == '7') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Insulin.png"); }
			if (Element == '8') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Crouch.png"); }
			if (Element == '9') { temp->Init("Scenery/maplane.x", "Scenery/TutorialBillboards/Success.png"); }
			temp->setPosition((-multiplier / 2) + x*multiplier, 40, (multiplier / 2) + z*multiplier*-1);
			temp->setScale(25.0 * (495.0/330.0), 25.0 * (330 / 495.0), 20);
			temp->setCollidable(false);
			temp->setAlive(true);
			temp->setRotation(0, -90, 90);
			temp->setEntityType(ENTITY_BILLBOARD);
			contents.push_back(temp);
			g_engine->addEntity(temp);
		}
		
	}
	void Scene::LazyRender(Effect* e, Camera* c) {

		// dont render billboards in the first "pass"
		for (size_t i = 0; i < contents.size(); i++) {
			if (contents[i]->getEntityType() != ENTITY_BILLBOARD) {
				contents[i]->LazyRender(e, c);
			}
		}
		// Render billboards after all other elements have been rendered, this allows the transparency to show other that are behind the billboard. 
		// Ultimate solution for this should be to sort the objects by the distance to the camera and render the furthest first
		for (size_t i = 0; i < contents.size(); i++) {
			if (contents[i]->getEntityType() == ENTITY_BILLBOARD) {
				contents[i]->LazyRender(e, c);
			}
		}
	}
	void Scene::Transform() {
		for (size_t i = 0; i < contents.size(); i++) {
			contents[i]->Transform();
		}
	}
	void Scene::Update(float deltaTime) {
		//sortBillboardsByDistance();
		for (size_t i = 0; i < contents.size(); i++) {
			contents[i]->Update(deltaTime);
		}
	}
}