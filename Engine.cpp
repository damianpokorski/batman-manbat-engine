#include "Engine.h"
using namespace std;

namespace Manbat{
	Engine::Engine(){
		p_apptitle = "Manbat Engine";
		p_screenwidth = 640;
		p_screenheight = 480;
		p_colordepth = 32;
		p_fullscreen = false;
		p_coreFrameCount =  0;
		p_coreFrameRate =  0;
		p_screenFrameCount =  0;
		p_screenFrameRate = 0;
		
		p_backdropColor = D3DCOLOR_XRGB(0,0,90);
		p_pauseMode = false;
		p_versionMajor = VERSION_MAJOR;
		p_versionMinor = VERSION_MINOR;
		p_revision = REVISION;
		p_commandLineParams = "";

		// Null surfaces
		p_MainSurface =  0;
		p_MainDepthStencilSurface = 0;

		p_windowHandle = 0;
	}
	Engine::~Engine(){
		if(p_input) p_input;
		if(p_device) p_device->Release();
		if(p_d3d) p_d3d->Release();
	}

	std::string Engine::GetVersionText(){
		std::ostringstream s;
		s << "Manbat the dank engine version " << p_versionMajor << "." << p_versionMinor << "." << p_revision;
		return s.str();
	}
	void Engine::Message(std::string message, std::string title){
		MessageBox(0,message.c_str(),title.c_str(),0);
	}
	void Engine::SetScreen(int w, int h, int d, bool fullscreen){
		// So we're using getter / setter methods here but not in the constructor? ummkay.
		SetScreenWidth(w);
		SetScreenHeight(h);
		SetColorDepth(d);
		SetFullscreen(fullscreen);
	}
	bool Engine::Init(HINSTANCE hInstance, int width, int height, int colordepth, bool fullscreen){
		playerController = NULL;


		string title;
		title = g_engine->GetAppTitle();

		RECT windowRect;
		windowRect.left = 0;
		windowRect.right = g_engine->GetScreenWidth();
		windowRect.top = 0;
		windowRect.bottom = g_engine->GetScreenHeight();

		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		// fill the struct with info
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = title.c_str();
		wc.hIconSm = NULL;
		//
		RegisterClassEx(&wc);
		DWORD dwStyle, dwExStyle;
		if(g_engine->GetFullscreen()){
			DEVMODE dm;
			memset(&dm,0,sizeof(dm));
			dm.dmSize = sizeof(dm);
			dm.dmPelsWidth = g_engine->GetScreenWidth();
			dm.dmPelsHeight = g_engine->GetScreenHeight();
			dm.dmBitsPerPel = g_engine->GetColorDepth();
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if(ChangeDisplaySettings(&dm,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL){
				Debug << "[Engine] Display mode change failed. Boo hoo." << std::endl;
				g_engine->SetFullscreen(false);
			}
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
		}else{
			dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}
		AdjustWindowRectEx(&windowRect,dwStyle, FALSE, dwExStyle);

		int wwidth = windowRect.right -windowRect.left;
		int wheight = windowRect.bottom -windowRect.top;

		Debug << "[Engine] Resolution" << width << ":" << height << std::endl;
		Debug << "[Engine] Creating window..." << std::endl;
		HWND hWnd = CreateWindowEx(
			0,
			title.c_str(),
			title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CW_USEDEFAULT,CW_USEDEFAULT,wwidth,wheight,
			NULL,NULL,
			hInstance,
			0);
		// If we've failed... Q_Q
		if(!hWnd){
			Debug << "[Engine] Couldn't create a window, we've dun goofed" << std::endl;
			return 0;
		}
		
		ShowWindow(hWnd,SW_SHOW);
		UpdateWindow(hWnd);

		g_engine->SetWindowHandle(hWnd);

		Debug << "[Engine] Creating th D3D Object Lets do dis." << std::endl;

		p_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(p_d3d==NULL){
			Debug << "[Engine] We've failed to create D3D object, gg no re." << std::endl;
			return 0;
		}
		D3DDISPLAYMODE dm;
		p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dm);

		
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = !fullscreen;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		d3dpp.EnableAutoDepthStencil = 1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 0;
		d3dpp.BackBufferWidth = GetScreenWidth();
		d3dpp.BackBufferHeight = GetScreenHeight();
		d3dpp.hDeviceWindow = hWnd;
		//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		// Lets get rid of them jaggedy edges, right?
		d3dpp.MultiSampleQuality = 0;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_8_SAMPLES;

		Debug << "[Engine] Creating Direct3D Device. Whoosh" << std::endl;
		
		p_d3d->CreateDevice(
			D3DADAPTER_DEFAULT, 
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&p_device
			);
		if(p_device==NULL){
			// Disable AA when on software rendering
			d3dpp.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE;
			Debug << "[Engine] Couldnt create hardware vertex. Lets try software..." << endl;
			p_d3d->CreateDevice(
				D3DADAPTER_DEFAULT, 
				D3DDEVTYPE_HAL,
				p_windowHandle,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&p_device
			);
			if(p_device==NULL){
				Debug << "[Engine] Cant even start software vertex. BB.";
				return 0;
			}
		}else{
			Debug << "[Engine] Hardware vertex processing activated. Launching photon torpedoes.";
		}
		Debug << "[Engine] Creating a 2D Renderer" << std::endl;

		HRESULT result = D3DXCreateSprite(p_device, &p_spriteObj);
		if(result!=D3D_OK){
			Debug << "[Engine] Couldnt create the D3DXSprite.. oh boi." << std::endl;
			return 0;
		}
		Debug << "[Engine] Initializing input system." << std::endl;
		p_input = new Input(GetWindowHandle());
		Debug<< "[Engine] Init xbone input system" << endl;
		p_padinput = new XboxPad(1);
		Debug << "[Engine] Initializing game:" << GetWindowHandle() << std::endl;
		if(!game_init(GetWindowHandle())) return 0;

		Debug << "[Engine] Successfully initialized the game. GG." << std::endl;
		// get the game window to appear on top and hide mouse cursor
		SetForegroundWindow(p_windowHandle);
		//ShowCursor(false);

		return 1;
	}

	void Engine::SetIdentity(){
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_engine->getDevice()->SetTransform(D3DTS_WORLD,&identity);
	}

	void Engine::SetSpriteIdentity(){
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_engine->getSpriteObject()->SetTransform(&identity);
	}
	void Engine::SavePrimaryRenderTarget(){
		p_device->GetRenderTarget(0,&p_MainSurface);
		p_device->GetDepthStencilSurface(&p_MainDepthStencilSurface);
	}

	void Engine::ClearScene(D3DCOLOR color){
		p_device->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f,0);
	}
	
	void Engine::Update(float elapsedTime){
		// Lock mouse cursor within window
		/*RECT windowpos;
		GetWindowRect(p_windowHandle, &windowpos);
		SetCursorPos(windowpos.left + (g_engine->GetScreenWidth() / 2), windowpos.top + (g_engine->GetScreenHeight() / 2));
*/
		Manbat::Timer Timer;
		static float accumTime=0;

		p_coreFrameCount++;
		if(p_coreTimer.Stopwatch(1000)){
			p_coreFrameRate = p_coreFrameCount;
			p_coreFrameCount = 0;
		}

		p_input->Update();
		UpdateKeyboard();
		UpdateMouse();
		game_update(elapsedTime);

		if(!p_pauseMode){
			//updateEntities(elapsedTime);
			testForCollisions();
		}

		if(!timedUpdate.Stopwatch(1)){
			timedUpdate.Rest(1);
		}else{
			// realframerate
			p_screenFrameCount++;
			if(p_screenTimer.Stopwatch(1000)){
				p_screenFrameRate = p_screenFrameCount;
				p_screenFrameCount = 0;
			}


			if(p_device->BeginScene()==D3D_OK){
				g_engine->ClearScene(g_engine->GetBackdropColor());
				//entitiesRender3D();
				game_render3D();
				p_spriteObj->Begin(D3DXSPRITE_ALPHABLEND);
				//entitiesRender2D();
				game_render2D();
				p_spriteObj->End();
				p_device->EndScene();
				p_device->Present(0,0,0,0);
			}
		}
		buryEntities();
	}
	void Engine::UpdateMouse(){
		
		static int oldPosX =0;
		static int oldPosY = 0;
		for(int n=0;n<4;n++){
			if(p_input->GetMouseButton(n)){
				RaiseEvent(new MouseClickEvent(n));
			}
		}
		int posx = p_input->GetMousePosX();
		int posy = p_input->GetMousePosY();
		if(posx != oldPosX || posy != oldPosY){
			oldPosX = posx;
			oldPosY = posy;
			RaiseEvent(new MouseMoveEvent(posx,posy));
		}
		int deltax = p_input->GetMouseDeltaX();
		int deltay = p_input->GetMouseDeltaY();

		if(deltax != 0 || deltay !=0){
			RaiseEvent(new MouseMotionEvent(deltax,deltay));
		}

		int wheel = p_input->GetMouseDeltaWheel();
		if(wheel!=0){
			RaiseEvent(new MouseWheelEvent(wheel));
		}
		
		// XBOX Update - lazymode
		
		XINPUT_STATE PadState = p_padinput->GetState();
		XboxPad* padPtr = p_padinput;
		RaiseEvent(new XButtonEvent(PadState, padPtr));

	}
	void Engine::UpdateKeyboard(){
		static Timer T;
		//Debug << " Keyboard check @" << T.getElapsedClock() << std::endl;
		static char old_keys[256];
		for(int n=0;n<255;n++){
			if(p_input->GetKeyState(n) & 0x80){
				old_keys[n] = p_input->GetKeyState(n);
				//Debug << "Key Down";
				RaiseEvent(new KeyPressEvent(n));
			}
			else {
				if (old_keys[n] & 0x80) {
					old_keys[n] = p_input->GetKeyState(n);
					//Debug << "Key Up";
					RaiseEvent(new KeyReleaseEvent(n));
				}
			}
		}
	}
	void Engine::Shutdown(){
		PostQuitMessage(0);
	}
	void Engine::RaiseEvent(IEvent* e){
		game_event(e);
		delete e;
	}

	void Engine::addEntity(Entity* entity){
		static int id = 0;
		entity->setID(id);

		p_entities.push_back(entity);

		id++;
		// Check if entity group is set - if not add vector with entity type
		if (SortedEntities.find(entity->entityType) == SortedEntities.end()) {
			vector<Entity*> entityContainer;
			SortedEntities.insert(std::pair<EntityType, vector<Entity*>>(entity->entityType, entityContainer));
		}
		// Add entity to the sorted group
		SortedEntities[entity->entityType].push_back(entity);
	}

	void Engine::updateEntities(float deltaTime){
		//BOOST_FOREACH(Entity* entity, p_entities){
		
		for(size_t i=0; i < p_entities.size(); i++){
			if(p_entities[i]->getAlive()){
				p_entities[i]->Update(deltaTime);
				RaiseEvent(new EntityUpdateEvent(p_entities[i]));
				if(p_entities[i]->getLifetime() > 0){
					p_entities[i]->addToLifetimeCounter(deltaTime*1000);
					if(p_entities[i]->lifetimeExpired()){
						p_entities[i]->setAlive(false);
					}
				}
			}
		}
		
	}

	void Engine::buryEntities(){
		// Erase from main entity container
		std::vector<Entity*>::iterator iter = p_entities.begin();
		while(iter!=p_entities.end()){
			if((*iter)->getAlive()==false){
				iter = p_entities.erase(iter);
			}else{
				iter++;
			}
		}
		// remove from the sorted array and then delete the pointer
		for (std::map<EntityType, vector<Entity*>>::iterator it = SortedEntities.begin(); it != SortedEntities.end(); ++it)
		{
			std::vector<Entity*>::iterator sorterIter = it->second.begin();
			while (sorterIter != it->second.end()) {
				if ((*sorterIter)->getAlive() == false) {
					sorterIter = it->second.erase(sorterIter);
					delete (*sorterIter);
				}
				else {
					sorterIter++;
				}
			}
		}
	}
	void Engine::entitiesRender3D(){
		//BOOST_FOREACH(Entity* entity, p_entities){
		//for(int i = 0; i < p_entities.size(); i++){
		//	if(p_entities[i]->getRenderType() == RENDER_3D){
		//		if(p_entities[i]->getAlive() && p_entities[i]->getVisible()){
		//			//entity->Render();
		//			//RaiseEvent(new EntityRenderEvent(entity));
		//		}
		//	}
		//}
	}
	void Engine::entitiesRender2D(){
		//BOOST_FOREACH(Entity* entity, p_entities){
		//	if(entity->getRenderType() == RENDER_2D){
		//		if(entity->getAlive() && entity->getVisible()){
		//			//entity->Render();
		//			//RaiseEvent(new EntityRenderEvent(entity));
		//		}
		//	}
		//}
	}
	Entity* Engine::findEntity(std::string name){
		for (size_t i = 0; i < p_entities.size(); i++) {
			if(p_entities[i]->getAlive() && p_entities[i]->getName() == name){
				return p_entities[i];
			}
		}
		return NULL;
	}
	Entity* Engine::findEntity(int id){
		for (size_t i = 0; i < p_entities.size(); i++) {
			if(p_entities[i]->getAlive() && p_entities[i]->getID() == id){
				return p_entities[i];
			}
		}
		return NULL;
	}
	int Engine::getEntityCount(enum EntityType entityType){
		int total = 0;
		for (size_t i = 0; i < p_entities.size(); i++) {
			if(p_entities[i]->getAlive() && p_entities[i]->getEntityType() == entityType){
				total++;
			}
		}
		return total;
	}

	void Engine::testForCollisions(){
		//Manbat::Timer timer;
		for (size_t i = 0; i < p_entities.size(); i++) {
			p_entities[i]->setCollided(false);
			p_entities[i]->setCollideBuddy(NULL);
		}
		//Debug << "[Collision] Entities: " << p_entities.size() << std::endl;
		int checks = 0;
		//if(!p_globalCollision) return;
		if(p_globalCollision)
		{
			for (size_t i = 0; i < p_entities.size(); i++) {
				if (p_entities[i]->getAlive() && p_entities[i]->isCollidable()) {
					for (size_t j = 0; j < p_entities.size(); j++) {
						if (p_entities[j]->getAlive() && p_entities[j]->isCollidable()) {
							if (Collision(p_entities[i], p_entities[j])) {
								p_entities[i]->setCollided(true);
								p_entities[i]->setCollideBuddy(p_entities[j]);
								p_entities[j]->setCollided(true);
								p_entities[j]->setCollideBuddy(p_entities[j]);
							}
							checks++;
						}
					}
				}
			}
		}else
		{
			// If scenery to player and enemies collision
			checks+= CollideEntityTypes(EntityType::ENTITY_PLAYER_MESH, EntityType::ENTITY_SCENERY_MESH);
			checks += CollideEntityTypes(EntityType::ENTITY_PLAYER_MESH, EntityType::ENTITY_MAINPLANE);
			checks += CollideEntityTypes(EntityType::ENTITY_PLAYER_MESH, EntityType::ENTITY_ENEMY_MESH);
			checks += CollideEntityTypes(EntityType::ENTITY_PLAYER_MESH, EntityType::ENTITY_FLAG);
			checks += CollideEntityTypes(EntityType::ENTITY_PLAYER_MESH, EntityType::ENTITY_COLLECTABLE_MESH);
			checks += CollideEntityTypes(EntityType::ENTITY_ENEMY_MESH, EntityType::ENTITY_SCENERY_MESH);
		}
		//Debug << "[Collision] Checked for: " << checks << " collisions." << std::endl;
		//Debug << "[Collision] Collision checks took: 1/" << (1.0f/timer.getElapsedClock()) << " of a second" << std::endl;

	}

	bool Engine::Collision(Entity* entity1, Entity* entity2){
		switch (entity1->getEntityType()){
			case ENTITY_PLAYER_MESH:
				switch(entity2->getEntityType()){
					case ENTITY_SCENERY_MESH:
						// collide only if distance is shorter than 50, a small optimization
						if (Math::Distance(((Mesh*)entity1)->getPosition(), ((Mesh*)entity2)->getPosition()) < 100) {
							return PlayerToSceneryCollision((Mesh*)entity1, (Mesh*)entity2);
						}
						return false;
					break;
					case ENTITY_COLLECTABLE_MESH:
						return Collision((Mesh*)entity1, (Mesh*)entity2);
					break;
					case ENTITY_MAINPLANE:
						return PlayerToSceneryCollision((Mesh*)entity1, (Mesh*)entity2);
					break;
					case ENTITY_ENEMY_MESH:
						if (Collision((Mesh*)entity1, (Mesh*)entity2)) {
							// Fire an event on top of calculating collision
							playerController->PlayerToEnemyCollision(((Mesh*)entity2));
							return true;
						}
						return false;
					break;
					case ENTITY_IMPROVED_COLLECTABLE_MESH:
						if (Collision((Mesh*)entity1, (Mesh*)entity2)) {
							// Fire an event on top of calculating collision
							playerController->PlayerToCollectableCollision(((ImprovedCollectable*)entity2));
							return true;
						}
						return false;
					case ENTITY_FLAG:
						if (Collision((Mesh*)entity1, (Mesh*)entity2)) {
							playerController->hitFlag = true;
							return true;
						}
						return false;
					break;
				}
			break;
			case ENTITY_ENEMY_MESH:
				switch (entity2->getEntityType()) {
				case ENTITY_SCENERY_MESH:
					if (Math::Distance(((Mesh*)entity1)->getPosition(), ((Mesh*)entity2)->getPosition()) < 100) {
						return PlayerToSceneryCollision((Mesh*)entity1, (Mesh*)entity2);
					}
					return false;
					break;
				}
			break;

			case ENTITY_SPRITE: 
				switch(entity2->getEntityType()){
					case ENTITY_SPRITE:
						return Collision((Sprite*)entity1,(Sprite*)entity2);
					break;
				}
			break;

			case ENTITY_MESH:
				switch(entity2->getEntityType()){
					case ENTITY_MESH:
						return false;
					break;
				}
			break;
			
			case ENTITY_RAY:
				switch(entity2->getEntityType()){
					case ENTITY_SCENERY_MESH:
						return RayAABBCollision((Mesh*)entity2, (Ray*)entity1);
					break;
					case ENTITY_PLAYER_MESH:
						return RayAABBCollision((Mesh*)entity2, (Ray*)entity1);
					break;
				}
			break;

			case ENTITY_SCENERY_MESH:
				return false;
			break;
		}
		return false;
	}

	bool Engine::Collision(Sprite* sprite1, Sprite* sprite2){
		Rect r1 = sprite1->getBounds();
		Rect r2 = sprite2->getBounds();
		return r1.Intersects(r2);
	}

	bool Engine::Collision(Mesh* mesh1, Mesh* mesh2){
		if (mesh1->getPosition().x + mesh1->getBoundingBox().min.x > mesh2->getPosition().x + mesh2->getBoundingBox().max.x) return false;
		if (mesh1->getPosition().x + mesh1->getBoundingBox().max.x < mesh2->getPosition().x + mesh2->getBoundingBox().min.x) return false;
		if (mesh1->getPosition().y + mesh1->getBoundingBox().min.y > mesh2->getPosition().y + mesh2->getBoundingBox().max.y) return false;		
		if (mesh1->getPosition().y + mesh1->getBoundingBox().max.y < mesh2->getPosition().y + mesh2->getBoundingBox().min.y) return false;
		if (mesh1->getPosition().z + mesh1->getBoundingBox().max.z < mesh2->getPosition().z + mesh2->getBoundingBox().min.z) return false;
		if (mesh1->getPosition().z + mesh1->getBoundingBox().min.z > mesh2->getPosition().z + mesh2->getBoundingBox().max.z) return false;
		return true;
	}
	bool Engine::EnemyToSceneryCollision(Mesh* Enemy, Mesh* scenery) {
		if (!Collision(Enemy, scenery)) {
			return false;
		}
		Enemy->RevertPosition();
		return true;
	}
	bool Engine::PlayerToSceneryCollision(Mesh* player, Mesh* scenery){
		if(!Collision(player,scenery)){
			return false;
		}
		Vector3 PrevPos = player->PreviousPosition;
		Vector3 CurrentPos = player->getPosition();
		// Sliding collision since you kno.. I'm a genius, right? Humble one too!
		
		// See if passes !collision just with Y & Z;
		player->setPosition(PrevPos.getX(), CurrentPos.getY(), CurrentPos.getZ());
		if (!Collision(player, scenery)) {
			return true;
		}

		// See if passes !collision just with X & Z;
		player->setPosition(CurrentPos.getX(), PrevPos.getY(), CurrentPos.getZ());
		if (!Collision(player, scenery)) {
			// Y had to be reset - hence player is probably grounded // I don't plan on adding any ceilings but if so this will be a problem :^]
			player->grounded = true;
			return true;
		}

		// See if passes !collision just with X & Y;
		player->setPosition(CurrentPos.getX(), CurrentPos.getY(), PrevPos.getZ());
		if (!Collision(player, scenery)) {
			return true;
		}

		// See if passes !collision just with X axis;
		player->setPosition(CurrentPos.getX(), PrevPos.getY(), PrevPos.getZ());
		if (!Collision(player, scenery)) {
			// Y had to be reset - hence player is probably grounded // I don't plan on adding any ceilings but if so this will be a problem :^]
			player->grounded = true;
			return true;
		}

		// See if passes !collision just with Z axis;
		player->setPosition(PrevPos.getX(), PrevPos.getY(), CurrentPos.getZ());
		if (!Collision(player, scenery)) {
			// Y had to be reset - hence player is probably grounded // I don't plan on adding any ceilings but if so this will be a problem :^]
			player->grounded = true;

			return true;
		}
		// See if passes !collision just with Y axis;
		player->setPosition(PrevPos.getX(), CurrentPos.getY(), PrevPos.getZ());
		if (!Collision(player, scenery)) {
			return true;
		}

		// Didnt pass any collision tests... lets just revert back to collision on frame before

		// Y had to be reset - hence player is probably grounded // I don't plan on adding any ceilings but if so, this will be a problem :^]
		player->grounded = true;


		player->PreviousPosition = PrevPos;
		player->RevertPosition();

		return true;
	}

	bool Engine::RayAABBCollision(Mesh* player, Ray* shot){

		Vector3 rayTestVec0 = shot->RayBegin;
		Vector3 rayTestVec1 = shot->RayBegin + shot->direction * 100;

		Vector3 BoxMin = player->getPosition()+player->getBoundingBox().min;
		Vector3 BoxMax = player->getPosition()+player->getBoundingBox().max;

		double fractionLow = 0;
		double fractionHigh = 1;

		double fractionLowX = (BoxMin.x - rayTestVec0.x) / (rayTestVec1.x - rayTestVec0.x);
		double fractionHighX = (BoxMax.x - rayTestVec0.x) / (rayTestVec1.x - rayTestVec0.x);

		if(fractionHighX < fractionLowX){
			swap(fractionLowX,fractionHighX);
		}
		if(fractionHighX < fractionLow){
			return false;
		}
		if(fractionLowX > fractionHigh){
			return false;
		}

		double fractionLowY = (BoxMin.y - rayTestVec0.y) / (rayTestVec1.y - rayTestVec0.y);
		double fractionHighY = (BoxMax.y - rayTestVec0.y) / (rayTestVec1.y - rayTestVec0.y);

		if(fractionHighY < fractionLowY){
			swap(fractionLowY,fractionHighY);
		}
		if(fractionHighY < fractionLow){
			return false;
		}
		if(fractionLowY > fractionHigh){
			return false;
		}

		double fractionLowZ = (BoxMin.z - rayTestVec0.z) / (rayTestVec1.z - rayTestVec0.z);
		double fractionHighZ = (BoxMax.z - rayTestVec0.z) / (rayTestVec1.z - rayTestVec0.z);

		if(fractionHighZ < fractionLowZ){
			swap(fractionLowZ,fractionHighZ);
		}
		if(fractionHighZ < fractionLow){
			return false;
		}
		if(fractionLowZ > fractionHigh){
			return false;
		}

		fractionLow = max(fractionLowZ,fractionLow);
		fractionHigh = min(fractionHighZ, fractionHigh);
		if(fractionLow > fractionHigh) return false;

		Vector3 raySection = rayTestVec1 - rayTestVec0;
		Vector3 intersection = rayTestVec0 + raySection * fractionLow;

		shot->intersectionPoint = intersection;

		return true;

	}

	// My mess - Caching 2D Textures
	Texture* Engine::cacheTexture(std::string filename){
		if(cacheTextureStorage.find(filename) != cacheTextureStorage.end()){
			return cacheTextureStorage.find(filename)->second;
		}
		Texture* Text = new Texture();
		if (!Text->Load(filename)) {
			Debug << "[Textures] Error cache-loading texture\"" << filename << "\"!" << std::endl;
			return 0;
		}
		cacheTextureStorage.insert(std::pair<std::string, Texture*>(filename, Text));
		return Text;
	}

	int Engine::CollideEntityTypes(EntityType Foo, EntityType Bar)
	{
		int CollisionChecks = 0;
		if (SortedEntities.find(Foo) == SortedEntities.end()) return CollisionChecks;
		if (SortedEntities.find(Bar) == SortedEntities.end()) return CollisionChecks;
		for (size_t i = 0; i < SortedEntities[Foo].size(); i++)
		{
			//if (SortedEntities[Foo][i]->getAlive() && SortedEntities[Foo][i]->isCollidable())
			//{
				for (size_t j = 0; j < SortedEntities[Bar].size(); j++)
				{
					//if (SortedEntities[Bar][j]->getAlive() && SortedEntities[Bar][j]->isCollidable())
					//{
						if (Collision(SortedEntities[Foo][i], SortedEntities[Bar][j]))
						{
							// assign properties
							SortedEntities[Foo][i]->setCollided(true);
							SortedEntities[Foo][i]->setCollideBuddy(SortedEntities[Bar][j]);
							SortedEntities[Bar][j]->setCollided(true);
							SortedEntities[Bar][j]->setCollideBuddy(SortedEntities[Foo][i]);
						}
					//}
					CollisionChecks++;
				}
			//}
		}
		return CollisionChecks;
	}
}
