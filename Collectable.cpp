#include "Engine.h"

using namespace Manbat;

Collectable::Collectable(int cType) :Entity(RENDER_NONE) {
	Init(cType);
}

Collectable::~Collectable() {
	if (mapIcon) delete mapIcon;
	if (texture) delete texture;
}

void Collectable::Init(int cType) {
	type = cType;

	collectableCollision = new Mesh();
	collectableCollision->createCube(2, 2, 2);

	texture = NULL;

	switch (type) {
	case COLLECT_SHURIKENS:
	{
		visibleModel = new Mesh();
		if (!visibleModel->Load("Collectables/shuriken.x")) {
			Debug << "Couldn't load shuriken" << std::endl;
		}
		mapIcon = new Sprite();
		mapIcon->Load("HUD/Map/shurikenMapSprite.png");
	}
	break;

	case COLLECT_DARTS:
	{
		visibleModel = new Mesh();
		if (!visibleModel->Load("Collectables/dart.x")) {
			Debug << "Couldn't load dart" << std::endl;
		}
		mapIcon = new Sprite();
		mapIcon->Load("HUD/Map/dartMapSprite.png");
	}
	break;

	case COLLECT_BOMBS:
	{
		visibleModel = new Mesh();
		if (!visibleModel->Load("Collectables/bomb.x")) {
			Debug << "Couldn't load dart" << std::endl;
		}
		mapIcon = new Sprite();
		mapIcon->Load("HUD/Map/bombMapSprite.png");
	}
	break;

	case COLLECT_STARS:
	{
		visibleModel = new Mesh();
		if(!visibleModel->Load("Collectables/star.x")) {
			Debug << "Couldn't load dart" << std::endl;
		}
		mapIcon = new Sprite();
		mapIcon->Load("HUD/Map/starMapSprite.png");
	}
	break;
	}


}