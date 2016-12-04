
#include "Engine.h"
using namespace std;
using namespace Manbat;
namespace Manbat {
	class GameLogic : public Screen
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
		float health;
		float energy;
		float insulin;
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
		GameLogic();
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
	};
};