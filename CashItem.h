#pragma once

#include "Engine.h"

namespace Manbat {

	enum cashType {
		CTC_ENEMYCAMP = 20,
		CTC_PLAYERCAMP = 30,
		CTC_CASH = 40
	};

	class CashItem : public Mesh
	{
	private:
		Texture* texture;
		Sprite*mapIcon;
		int type;
	public:
		CashItem(int cType);
		~CashItem();

		void Init(int cType);
		int getType() { return type; }
		Texture* getTexture() { return texture; }
		Sprite* getMap() { return mapIcon; }

	};


};