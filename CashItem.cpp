#include "Engine.h"

using namespace Manbat;

CashItem::CashItem(int cType)
{
	Init(cType);
}

CashItem::~CashItem()
{
	if (mapIcon) delete mapIcon;
	if (texture) delete texture;
}

void CashItem::Init(int cType)
{

	type = cType;

	switch (type)
	{
	case CTC_ENEMYCAMP:
	{
		texture = new Texture();
		texture->Load("enemycamp.png");
		mapIcon = new Sprite();
		mapIcon->Load("enemycampmap.png");
	}
	break;
	case CTC_PLAYERCAMP:
	{
		texture = new Texture();
		texture->Load("playercamp.png");
		mapIcon = new Sprite();
		mapIcon->Load("playercampmap.png");
	}
	break;
	case CTC_CASH:
	{
		texture = new Texture();
		texture->Load("cash.png");
		mapIcon = new Sprite();
		mapIcon->Load("cash.png");
	}
	break;


	}
}
