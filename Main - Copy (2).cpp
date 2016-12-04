#include "Engine.h"
using namespace std;
using namespace Manbat;


Effect* effect;
Effect* skyboxEffect;
Skybox* skybox;
Camera* camera;
Mesh* floorBox;
Mesh* playerCollision;
Texture* terrainTex;
Font* debugText;
Sprite* mapBg;
Sprite* charMap;
Sprite* HUDBg;
Sprite* NinjaFaceAnim;

std::vector<Entity*> sceneryBoxes;
std::vector<Entity*> sceneryBoxesMap;

Vector3 currentVelocity;
Vector3 goalVelocity;

EAngle characterToCamera;
Vector3 charMovement;
Vector3 vecForward;
Vector3 vecRight;
bool charMoving = false;

int gameState;
#define GAME_PLAY_STATE 1

int LeftX = 0;
int LeftY = 0;
int RightX = 0;
int RightY = 0;

struct line_vertex {
	float x, y, z;
	DWORD color;
};


D3DXVECTOR4 RainbowReturn();
float Approach(float flGoal, float flCurrent, float dt) {
	float flDifference = flGoal - flCurrent;

	if (flDifference > dt)
		return flCurrent + dt;
	if (flDifference < -dt)
		return flCurrent - dt;

	return flGoal;
}
Vector2 ObjectToMap(Vector3 obj) {
	Vector2 newpos;
	newpos.x = mapBg->getPosition().x + (floor(obj.x / 500 * 200));
	newpos.y = mapBg->getPosition().y + (-floor(obj.z / 500 * 200));
	return newpos;
}

bool game_preload() {
	g_engine->SetAppTitle("I hope this works...");

	g_engine->SetScreen(1600, 900, 32, false);

	return true;
}

bool game_init(HWND hwnd) {
	skyboxEffect = new Effect();
	if (!skyboxEffect->Load("Shaders/skybox.fx")) {
		Debug << "Error loading skybox effect";
		return false;
	}

	skybox = new Skybox();
	if (!skybox->Create("Skybox.png")) {
		Debug << "Error loading skybox";
		return false;
	}

	camera = new Camera();
	characterToCamera.p = 0.0f;
	characterToCamera.y = 0.0f;
	characterToCamera.r = 0.0f;

	camera->setPosition(400, 1, -140);
	camera->setTarget(1, 0, 0);
	camera->Update();

	playerCollision = new Mesh();
	playerCollision->createCube(4, 4, 4);
	playerCollision->setPosition(camera->getPosition());
	playerCollision->setEntityType(ENTITY_PLAYER_MESH);
	playerCollision->setAlive(true);
	playerCollision->setCollidable(true);
	g_engine->addEntity(playerCollision);

	floorBox = new Mesh();
	if (!floorBox->Load("Scenery/mainplane.x")) {
		Debug << "Error loading floorBox file" << std::endl;
		return false;
	}

	effect = new Effect();
	if (!effect->Load("Shaders/ambient_textured.fx")) {
		Debug << "Error loading effect file" << std::endl;
		return false;
	}

	terrainTex = new Texture();
	if (!terrainTex->Load("Scenery/newtex.jpg")) {
		Debug << "Error loading terrain tex!" << std::endl;
		return false;
	}

	debugText = new Font("Arial Bold", 30);

	if (!debugText) {
		Debug << "Error creating font" << endl;
		return false;
	}

	Debug << "Font ok" << endl;

	mapBg = new Sprite();
	if (!mapBg->Load("HUD/Map/MapBGSprite.png")) {
		Debug << "Error creating a map" << std::endl;
		return false;
	}
	mapBg->setPosition(1300, 100);

	charMap = new Sprite();
	if (!charMap->Load("HUD/Map/ninjaMapSprite.png")) {
		Debug << "Error char map sprite" << std::endl;
		return false;
	}

	charMap->setPivot(Vector2(5, 5));
	charMap->setPosition(1302, 102);

	HUDBg = new Sprite();
	if (!HUDBg->Load("HUD/HUDbgSprite.png")) {
		Debug << "Error creating HUD BG" << std::endl;
		return false;
	}

	HUDBg->setPosition(0, 800);

	NinjaFaceAnim = new Sprite();
	if (!NinjaFaceAnim->Load("HUD/NinjaFaceAnim.png")) {
		Debug << "Error creating ninjafaceanim" << std::endl;
		return false;
	}

	NinjaFaceAnim->setPosition(750, 800);
	NinjaFaceAnim->animationColumns = 10;
	NinjaFaceAnim->currentFrame = 0;
	NinjaFaceAnim->setSize(100, 100);
	NinjaFaceAnim->lastFrame = 3;
	NinjaFaceAnim->animationDirection = 2;

	sceneryBoxes.push_back(new Mesh());
	sceneryBoxesMap.push_back(new Sprite());

	// Mesh* meshPtr = (Mesh*)sceneryBoxes.back(); HAS TO GO
	if (!((Mesh*)sceneryBoxes.back())->Load("Scenery/bush.x")) {
		Debug << "Error loading bush rock object" << std::endl;
		return false;
	}

	((Sprite*)sceneryBoxesMap.back())->Load("HUD/Map/bushMap.png");
	((Sprite*)sceneryBoxesMap.back())->setPivot(Vector2(2, 2));
	((Sprite*)sceneryBoxesMap.back())->setPosition(ObjectToMap(((Mesh*)sceneryBoxes.back())->getPosition()));

	((Mesh*)sceneryBoxes.back())->setPosition(350, 0, -140);
	((Mesh*)sceneryBoxes.back())->setScale(1);
	// ((Sprite*)sceneryBoxesMap.back())
	((Mesh*)sceneryBoxes.back())->setCollidable(true);
	((Mesh*)sceneryBoxes.back())->setAlive(true);
	((Mesh*)sceneryBoxes.back())->setEntityType(ENTITY_SCENERY_MESH);
	g_engine->addEntity(((Mesh*)sceneryBoxes.back()));

	gameState = GAME_PLAY_STATE;
	// CONTROLLER
	return true;
}

void game_end() {
	for (int i = 0; i < sceneryBoxesMap.size(); i++) {
		if (sceneryBoxesMap[i]) delete sceneryBoxesMap[i];
	}
	for (int i = 0; i < sceneryBoxes.size(); i++) {
		if (sceneryBoxes[i]) delete sceneryBoxes[i];
	}
	if (charMap) delete charMap;
	if (mapBg) delete mapBg;
	if (debugText) delete debugText;
	if (terrainTex) delete terrainTex;
	if (effect) delete effect;
	if (floorBox) delete floorBox;
	if (playerCollision) delete playerCollision;
	if (camera) delete camera;
	if (skybox) delete skybox;
	if (skyboxEffect) delete skyboxEffect;
}

void game_update(float deltaTime) {
	if (gameState == GAME_PLAY_STATE) {
		Vector3 camTarg = characterToCamera.ToVector();
		camera->setPosition(playerCollision->getPosition());
		camera->setTarget(camera->getPosition() + camTarg);

		charMovement.setX(Approach(goalVelocity.getX(), charMovement.getX(), deltaTime * 20));
		charMovement.setZ(Approach(goalVelocity.getZ(), charMovement.getZ(), deltaTime * 20));

		vecForward = characterToCamera.ToVector();
		vecForward.y = 0;
		vecForward = Math::Normal(vecForward);

		Vector3 vecUp(0, 1, 0);

		vecRight = Math::crossProduct(vecUp, vecForward);

		currentVelocity = vecForward * -charMovement.z + vecRight *-charMovement.x;
		// MY TEST
		if (playerCollision->PositionReseted) {
			camera->setPosition(playerCollision->PreviousPosition);
			playerCollision->PositionReseted = false;
		}
		// MY TEST END
		camera->setPosition(
			camera->getPosition().getX() + currentVelocity.getX() * deltaTime,
			camera->getPosition().getY() + currentVelocity.getY() * deltaTime,
			camera->getPosition().getZ() + currentVelocity.getZ() * deltaTime
			);
		// Player collision position is inherited from camera? that's fishy
		playerCollision->setPosition(camera->getPosition());
		playerCollision->Update(deltaTime);

		if (currentVelocity.x == 0 && currentVelocity.z == 0) {
			charMoving = false;
			NinjaFaceAnim->firstFrame = 0;
			NinjaFaceAnim->lastFrame = 3;
		}
		else {
			charMoving = true;
			NinjaFaceAnim->firstFrame = 3;
			NinjaFaceAnim->lastFrame = 6;
		}

		charMap->setPosition(ObjectToMap(playerCollision->getPosition()));

		NinjaFaceAnim->Update(deltaTime);

		for (int i = 0; i < sceneryBoxes.size(); i++) {
			Mesh* boxPtr = (Mesh*)sceneryBoxes[i];
			boxPtr->Update(deltaTime);
			Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
			sprPtr->setPosition(ObjectToMap(boxPtr->getPosition()));
		}
	}

	camera->Update();
	floorBox->Transform();
}

void game_render3D() {
	/*Matrix matWorld;
	matWorld.setIdentity();
	skyboxEffect->setWorldMatrix(matWorld);
	skyboxEffect->setViewMatrix(camera->getViewMatrix());
	skyboxEffect->setProjectionMatrix(camera->getProjMatrix());

	skyboxEffect->Begin();
	skybox->Render();
	skyboxEffect->End();*/

	effect->setProjectionMatrix(camera->getProjMatrix());
	effect->setViewMatrix(camera->getViewMatrix());
	effect->setWorldMatrix(floorBox->getMatrix());
	effect->setParam("Texture", terrainTex->getTexture());
	floorBox->Render(effect);

	for (int i = 0; i < sceneryBoxes.size(); i++) {
		Mesh* boxPtr = (Mesh*)sceneryBoxes[i];
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(boxPtr->getMatrix());
		boxPtr->Render(effect);
	}
}

void game_render2D() {
	ostringstream os;
	os.imbue(std::locale("english-us"));

	os << "Left X:" << LeftX << std::endl;
	os << "Left Y:" << LeftY << std::endl;
	os << "Right X:" << RightX << std::endl;
	os << "Right Y:" << RightY << std::endl;

	os << "Character" <<
		" pos X" << playerCollision->getPosition().getX() <<
		" pos Y" << playerCollision->getPosition().getY() <<
		" pos Z" << playerCollision->getPosition().getZ() <<
		std::endl;



	os << "Character to box normal X: " << Math::Normal(((Mesh*)sceneryBoxes[0])->getPosition() - ((Mesh*)playerCollision)->getPosition()).getX() <<
		"Y: " << Math::Normal(((Mesh*)sceneryBoxes[0])->getPosition() - ((Mesh*)playerCollision)->getPosition()).getY() <<
		"Z: " << Math::Normal(((Mesh*)sceneryBoxes[0])->getPosition() - ((Mesh*)playerCollision)->getPosition()).getZ();


	debugText->Print(50, 50, os.str());

	LeftX = 0;
	LeftY = 0;
	RightX = 0;
	RightY = 0;

	mapBg->Render();
	charMap->Render();
	for (int i = 0; i < sceneryBoxes.size(); i++) {
		Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
		sprPtr->Render();
	}

	HUDBg->Render();
	NinjaFaceAnim->Render();


}


void game_event(IEvent* e) {
	switch (e->GetID()) {
	case EventType::EVENT_KEYPRESS:
		if ((((KeyPressEvent*)e)->keycode) == DIK_ESCAPE) {
			g_engine->Shutdown();
		}
		switch (((KeyPressEvent*)e)->keycode) {
		case DIK_W:
			goalVelocity.setZ(-10);
			break;
		case DIK_S:
			goalVelocity.setZ(10);
			break;
		case DIK_A:
			goalVelocity.setX(10);
			break;
		case DIK_D:
			goalVelocity.setX(-10);
			break;

		}
		break;
	case EventType::EVENT_KEYRELEASE:
		if ((((KeyReleaseEvent*)e)->keycode) == DIK_ESCAPE) {
			g_engine->Shutdown();
		}
		switch (((KeyReleaseEvent*)e)->keycode) {
		case DIK_W:
			goalVelocity.setZ(0);
			break;
		case DIK_S:
			goalVelocity.setZ(0);
			break;
		case DIK_A:
			goalVelocity.setX(0);
			break;
		case DIK_D:
			goalVelocity.setX(0);
			break;

		case DIK_E:
			Debug << "TP!!!" << std::endl;
			break;
		}
		break;
	case EventType::EVENT_MOUSEMOTION:
	{
		MouseMotionEvent* mme = (MouseMotionEvent*)e;
		float movex = mme->deltax;
		float movey = mme->deltay;

		if (gameState == GAME_PLAY_STATE) {
			characterToCamera.y -= movex * 0.0075;
			characterToCamera.p -= movey * 0.0075;
		}

		if (characterToCamera.p >= 1.565)
			characterToCamera.p = 1.565;
		if (characterToCamera.p <= -1.565)
			characterToCamera.p = -1.565;

		characterToCamera.Normalize();
	}
	break;

	case EventType::EVENT_XBUTTON: {
		XButtonEvent* xbe = (XButtonEvent*)e;
		LeftX = xbe->PadState.Gamepad.sThumbLX;
		LeftY = xbe->PadState.Gamepad.sThumbLY;
		RightX = xbe->PadState.Gamepad.sThumbRX;
		RightY = xbe->PadState.Gamepad.sThumbRY;

		//goalVelocity.setX((LeftX / 32500)*-10);
		//goalVelocity.setZ((LeftY / 32500)*-10);

		characterToCamera.y -= (RightX / 32500) * 0.03;
		characterToCamera.p -= (RightY / 32500) * 0.03;
	}
	}
}
float Red = 1.0f;
float Green = 0.0f;
float Blue = 0.0f;
int ColorM = 1;
D3DXVECTOR4 RainbowReturn() {
	switch (ColorM) {
	case 0:
		if (Red < 1) {
			Red += 0.01f;
			if (Blue > 0) {
				Blue -= 0.01f;
			}
		}
		else {
			Red = 1;
			ColorM++;
		}
		break;
	case 1:
		if (Green < 1) {
			Red -= 0.01f;
			Green += 0.01f;
		}
		else {
			Green = 1;
			Red = 0;
			ColorM++;
		}
		break;
	case 2:
		if (Blue < 1) {
			Green -= 0.01f;
			Blue += 0.01f;
		}
		else {
			Blue = 1;
			Red = 0;
			ColorM = 0;
		}
		break;
	}
	return D3DXVECTOR4(Red, Green, Blue, 1);
};

