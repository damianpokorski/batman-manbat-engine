// I heard this is a bad practice...
#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>			// Windowstuff
#include <iostream>				

#include <map>					// the overcomplicated version of dictionaries from c# Q_Q
#include <list>					// YEA BOI, bye bye arrays
#include <vector>

#include <string>
#include <sstream>				// Wish we could just get ToString() from C#...
								// Edit: Just got to the line 71... bollocks.

#include <fstream>				// Gotta use them files
#include <iomanip>				// more like "ioturnip" amirite

#include <ctime>				// You can't "see" the time.

#include <cstdio>				// How much more IO stuff do we need?
#include <cstdlib>				// Standard library functions? Oh its rand, system  etc.
#include <cmath>				// Math stuff, is there a DoMath()  ?

#include <io.h>					// I guess we'll be doing a lot of inputting and outputtin?
#include <algorithm>			// extra vector / map functions

// DirectX headers
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>	
#include <d3dx9.h>
#include <dinput.h> // DirectX Input
#include <D3dx9shader.h>
#include <XInput.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <cmath>
#include "tinyxml2.h"
// Engine stuff

#include "Entity.h"
#include "Font.h"
#include "Timer.h"
#include "Input.h"
#include "XboxPad.h"
#include "SoundLayer.h"
#include "Sound.h"
#include "Event.h"
#include "LogFile.h"
#include "Vector.h"
#include "Math.h"
#include "EAngle.h"
#include "Matrix.h"
#include "Camera.h"
#include "Effect.h"
#include "Color.h"
#include "Texture.h"
#include "Mesh.h"
#include "Skybox.h"
#include "BoneMesh.h"
#include "Rect.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "BitmapFont.h"
#include "Ray.h"
#include "Collectable.h"

#include "SaveGame.h"
#include "ImprovedMesh.h"
#include "Billboard.h"
#include "Enemy.h"
#include "ImprovedCollectable.h"
#include "AnimatedBillboard.h"
#include "ImprovedSkybox.h"
#include "ImprovedCamera.h"
#include "Scene.h"
#include "Screen.h"
#include "StartScreen.h"
#include "LevelSelectionScreen.h"
#include "PauseScreen.h"
#include "LoadingScreen.h"
#include "CreditsScreen.h"
#include "GameLogic.h"
#include "CharacterLoader.h"
#include "ScarletSkylineGameLogic.h"

// Load libraries
#pragma comment(lib, "d3d9.lib")		// d3d
#pragma comment(lib, "d3dx9.lib")		// Expanding on d3d!
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dinput8.lib")		// Gotta handle the input 
#pragma comment(lib, "xinput.lib")		// Controller stuff i guess?
#pragma comment(lib, "dxguid.lib")		
#pragma comment(lib, "winmm.lib")		// Please tell me we've got an easier way of playing sound than last year. I dont want to see a 50page word document.
#pragma comment(lib, "user32.lib")		// UI stuff
#pragma comment(lib, "gdi32.lib")		// We're drawing things! Lemme get my crayons

// Version definitions?
#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define REVISION 0

// global functions? last time i checked we were doing c++ not php.. killmenow.jpg
extern bool game_preload();
extern bool game_init(HWND hwnd);
extern void game_update(float dltTime);
extern void game_render3D();
extern void game_render2D();
extern void game_event(Manbat::IEvent* e);
extern void game_end();
// Windows callback
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Manbat{
	// THIS WAS A THING!? Why did no one tell me about this. I needed it so much last year. FML.
	template <class T>std::string static ToString(const T & t, int places = 2){
		std::ostringstream oss;
		oss.precision(places);
		oss.setf(std::ios_base::fixed);
		oss << t;
		return oss.str();
	}

	struct MeshData{
		LPD3DXMESH Mesh;
		DWORD material_count;
		D3DMATERIAL9* materials;
		LPDIRECT3DTEXTURE9* textures;
	};
	struct TextureData {
		LPDIRECT3DTEXTURE9 Texture;
	};

	class Engine{
		// Everything is public.. i'm lazy.
		public:
			// texture and mesh caching
			std::map<char*, MeshData> MeshCache;
			bool MeshExists(char* filename) {
				return !(MeshCache.find(filename) == MeshCache.end());
			};
			std::map<string, TextureData> TextureCache;
			bool TextureExists(string filename) {
				return !(TextureCache.find(filename) == TextureCache.end());
			};
			// other variables
			std::string p_commandLineParams;
			int p_versionMajor, p_versionMinor, p_revision;
			HWND p_windowHandle;
			LPDIRECT3D9 p_d3d;
			LPDIRECT3DDEVICE9 p_device;
			LPD3DXSPRITE p_spriteObj;
			std::string p_apptitle;
			bool p_fullscreen;
			int p_screenwidth;
			int p_screenheight;
			int p_colordepth;
			bool p_pauseMode;
			Timer p_coreTimer;
			long p_coreFrameCount;
			long p_coreFrameRate;
			Timer p_screenTimer;
			long p_screenFrameCount;
			long p_screenFrameRate;
			Timer timedUpdate;
			D3DCOLOR p_backdropColor;

			// Surface pointers
			LPDIRECT3DSURFACE9 p_MainSurface;
			LPDIRECT3DSURFACE9 p_MainDepthStencilSurface;
			// Im so lazy...
			// Pointer to the player controller
			ImprovedCamera* playerController;

			// Input stuff
			Input *p_input;
			XboxPad *p_padinput;
			D3DPRESENT_PARAMETERS d3dpp;
			void UpdateKeyboard();
			void UpdateMouse();

			Engine();
			virtual ~Engine();
			bool Init(HINSTANCE hInstance, int width, int height, int colordepth, bool fullscreen);
			void Update(float deltaTime);
			void Message(std::string message, std::string title="FATAL ERROR");
			void Shutdown();
			void ClearScene(D3DCOLOR color);
			// Who am i?!
			void SetIdentity();
			void SetSpriteIdentity();
			// They may take our lives but they will never take our freedom!
			int Release();
			// Manipulate stuff!
			void SavePrimaryRenderTarget();
			void RestorePrimaryRenderTarget();
			//
			bool IsPaused() { return p_pauseMode; };
			bool SetPaused(bool value){ p_pauseMode = value; };
			// 
			LPDIRECT3D9 GetObject() { return p_d3d; };
			LPDIRECT3DDEVICE9 getDevice() { return p_device; };
			LPD3DXSPRITE getSpriteObject() { return p_spriteObj; };

			void SetWindowHandle(HWND hwnd){ p_windowHandle = hwnd; };
			HWND GetWindowHandle(){ return p_windowHandle; };

			std::string GetAppTitle() { return p_apptitle; };
			void SetAppTitle(std::string value){ p_apptitle = value; };

			int GetVersionMajor() { p_versionMajor; };
			int GetVersionMinor() { p_versionMinor; };
			int GetRevision() { p_revision; };
			std::string GetVersionText();

			long GetCoreFrameRate(){ return p_coreFrameRate; };
			long GetScreenFrameRate(){ return p_screenFrameRate; };

			void SetScreen(int w, int h, int d, bool full);
			int GetScreenWidth() { return p_screenwidth; };
			void SetScreenWidth(int value){ p_screenwidth = value; };
			int GetScreenHeight() { return p_screenheight; };
			void SetScreenHeight(int value){ p_screenheight = value; };
			int GetColorDepth(){ return p_colordepth; };
			void SetColorDepth(int value){ p_colordepth = value; };
			bool GetFullscreen(){ return p_fullscreen; };
			void SetFullscreen(bool value){ p_fullscreen = value; };

			D3DCOLOR GetBackdropColor(){ return p_backdropColor; };
			void SetBackground(D3DCOLOR value){ p_backdropColor = value; };

			std::string GetCommandLineParams() { return p_commandLineParams; };
			void SetCommandLineParams(std::string value){ p_commandLineParams = value; };

			void RaiseEvent(IEvent*);

			bool p_globalCollision;
			std::vector<Entity*> p_entities;
			void updateEntities(float deltaTime);
			void drawEntities();
			void entitiesRender3D();
			void entitiesRender2D();

			void addEntity(Entity* entity);
			int getEntityCount(enum EntityType entityType);
			Entity* findEntity(int id);
			Entity* findEntity(std::string name);
			void buryEntities();

			void enableGlobalCollisions(){ p_globalCollision = true;};
			void disableGlobalCollisions(){ p_globalCollision = false;};
			void setGlobalCollisions(bool value){p_globalCollision = value;};
			bool getGlobalCollisions(){return p_globalCollision;};

			void testForCollisions();
			bool Collision(Entity* entity1, Entity* entity2);
			bool Collision(Sprite* sprite1, Sprite* sprite2);
			bool Collision(Mesh* mesh1, Mesh* mesh2);

			bool EnemyToSceneryCollision(Mesh * Enemy, Mesh * scenery);

			bool PlayerToSceneryCollision(Mesh* player, Mesh* scenery);

			bool RayAABBCollision(Mesh* player, Ray* shot);
			// My mess
			
			Texture* cacheTexture(std::string filename);
			std::map<std::string, Texture*> cacheTextureStorage;

			std::map<EntityType, vector<Entity*>> SortedEntities;

			int CollideEntityTypes(EntityType Foo, EntityType Bar);
	};
};

// Global stuff. EZ.
extern std::auto_ptr<Manbat::Engine> g_engine;
