#pragma once

#include "Engine.h"

namespace Manbat
{
	class Ray : public Entity
	{
	public:
		D3DXVECTOR3 RayBegin;						//origin of ray
		D3DXVECTOR3 direction;						//direction of ray
		float collisionDistance;							//distance from origin where collided
		Vector3 intersectionPoint;						// point where ray hits
		bool player;										//fired from player?

		//constructor destructor
		Ray();
		~Ray();

		//init - takes in origin, direction and whether player or not
		void Init(Vector3 startFrom, Vector3 target, bool nPlayer);

		//update  / render
		void Update(float deltaTime);
		void Render();

	};
}