#pragma once

#include "Engine.h"


namespace Manbat {
	enum collectableType {
		COLLECT_SHURIKENS = 10,
		COLLECT_DARTS = 20,
		COLLECT_BOMBS = 30,
		COLLECT_STARS = 40
	};

	class Collectable : public Entity {
	private:
		Texture* texture;
		Sprite* mapIcon;
		int type;
		Mesh* visibleModel;
		Mesh* collectableCollision;
	public:
		Collectable(int cType);
		~Collectable();

		void Update(float deltaTime) {}
		void Render() {}

		void Init(int cType);
		int getType() { return type; }
		Texture* getTexture() { return texture; }
		Sprite* getMap() { return mapIcon; }
		void setMap(Sprite* spr) { mapIcon = spr; }
		Mesh* getVisibleMesh() { return visibleModel; }
		Mesh* getInvisibleMesh() { return collectableCollision; }


	};
}
