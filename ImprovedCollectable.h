#pragma once
#include "Engine.h"
namespace Manbat {
	class ImprovedCollectable : public ImprovedMesh {
	public:
		enum CollectableType {
			None,
			Health,
			Energy,
			Insulin,
			Shotgun,
			Revolver,
			TwoByFour,
			Molotov,
			TNT,
			SHELLS,
			BULLETS,
			BANDAGES,
			JAHNWAYNE
		};
		float animProgress;
		bool direction;
		CollectableType collectableType;
		ImprovedCollectable() {
			animProgress = 0;
			direction = true;
		};

		void Update(float deltaTime) {
			if (Disabled)
				return;
			// Animate up and down
			if (animProgress > 1) {
				direction = false;
			}
			if (animProgress < 0) {
				direction = true;
			}
			if (direction) {
				animProgress += deltaTime;
			}
			else {
				animProgress -= deltaTime;
			}

			this->setPosition(
				this->getPosition().getX(),
				15 + 5 * animProgress,
				this->getPosition().getZ()
				);
			this->Rotate(deltaTime * 180, 0,0);
			ImprovedMesh::Update(deltaTime);
		}

		void Disable() {
			this->setPosition(this->getPosition().getX(),-50,this->getPosition().getZ());
		};
	};
};