#include "Engine.h"
using namespace std;

namespace Manbat
{
	//constructor / entity render type
	Ray::Ray() :Entity(RENDER_3D)
	{
		entityType = ENTITY_RAY;

	}

	//init - takes in origin, directin and whether player or not
	void Ray::Init(Vector3 startFrom, Vector3 dir, bool nPlayer){

		//set properties for begining and direction of ray
		RayBegin = startFrom.ToD3DXVECTOR3();
		direction = Math::Normal(dir).ToD3DXVECTOR3();

		//Collision detection properties 
		//where the ray hits
		collisionDistance = 0;
		intersectionPoint = Vector3(0, 0, 0);

		//set player flag
		player = nPlayer;
	}

	Ray::~Ray(){//BYE ray
	}

	void Ray::Update(float deltaTime){ // not really much to update
	}

	void Ray::Render(){ // this shouldn't be called
	}
};