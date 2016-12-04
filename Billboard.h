#pragma once
#include "Engine.h"
namespace Manbat {
	class Billboard : public ImprovedMesh {
	public:
		ImprovedMesh* AttachedTo;
		Vector3 AttachedOffset;
		Billboard() {
			AttachedTo = NULL;
			AttachedOffset = Vector3(0, 38, 0);
			setPivot(Vector3(1.0f, 0.0f, 0.5f));
		};
		
		void rotateBillboard(Camera* cam) {
				float angleZ = 0;
				//float deltaX = this->getPosition().getX() - cam->getPosition().getX();
				float deltaX = (this->getPosition().getX() + this->getScale().getX()) - cam->getPosition().getX();
				float deltaY = (this->getPosition().getY() + this->getScale().getY()) - cam->getPosition().getY(); 
				float deltaZ = (this->getPosition().getZ() + this->getScale().getZ()) - cam->getPosition().getZ();
				angleZ = atan2(deltaX, deltaZ) * 180 / D3DX_PI;
				float angleX = atan2(deltaY, deltaZ) * 180 / D3DX_PI;
				float angleY = atan2(deltaX, deltaY) * 180 / D3DX_PI;
				this->setRotation(0, -90, angleZ);

				if (AttachedTo) {
					this->setPosition(AttachedTo->getPosition() + AttachedOffset);
				}
		}

		void Update(float deltaTime) {
			Mesh::Update(deltaTime);
		};
		void Transform() {
			ImprovedMesh::Transform();
		};
	};
}; 
