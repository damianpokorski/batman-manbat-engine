#pragma once
#include "Engine.h"
using namespace std;
using namespace Manbat;
namespace Manbat {
	class ScarletSkylineGameLogic : public Screen
	{
	public:
		enum Levels {
			Tutorial,
			One,
			Two
		};
		// Camera 
		ImprovedCamera* cam;
		float cameraWobble;

		// Font
		Font* font;
		Font* fontSmall;
		D3DCOLOR fontColor;
		bool DebugMode;

		// Player values
		bool sprinting;
		// Scene
		Scene* scene;
		ImprovedMesh* mainplane;
		int currentLevel;

		// Shader
		Effect* shader;

		// Skybox
		ImprovedSkybox* newSkybox;

		// Distraction / Dizziness
		float sinPower;
		PauseScreen* pScreen;

		// Debug
		bool FPSDisplay;
		ScarletSkylineGameLogic();
		// BGM SOUND
		cSound* BGM;

		virtual void Init();
		virtual void Update(float deltaTime);
		virtual void Render2D();
		virtual void Render3D();

		void InputEvent(IEvent* e);
		bool reloadTrigger;
		int reloadTarget;
		void reloadLevel(int levelID = 0);
		// Minimap
		Sprite* MiniMap;
		Sprite* MiniMapPlayer;
		Vector2 ObjectToMap(Vector3 V)
		{
			Vector2 newPos;
			newPos.x = MiniMap->getPosition().x + (floor(V.x / 10));
			newPos.y = MiniMap->getPosition().y + (-floor(V.z / 10));
			return newPos;
		}
		// Variables
		
		bool shotgunGotFlag = false;
		bool revolverGotFlag = false;
		bool woodFlag = false;
		int molotovs = 0;
		int tnt = 0;
		int bullets = 0;
		int shells = 0;
		int health = 100;
		BitmapFont* westernFonts;
	};
};
