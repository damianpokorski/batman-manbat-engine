#pragma once
#include "Engine.h"
namespace Manbat {
	class AnimatedBillboard : public Billboard {
	public:
		std::vector<Texture*> frames;
		int currentFrame;
		float FPS;
		float frameDuration;
		AnimatedBillboard() {
			setPivot(Vector3(1.0f, 0.0f, 0.0f));
			currentFrame = 0;
			FPS = 24;
			frameDuration = 0;
		};
		
		void rotateBillboard(Camera* cam) {
			double angleZ = 0;
			double deltaX = this->getPosition().getX()+this->getScale().getX() - cam->getPosition().getX();
			double deltaZ = (this->getPosition().getZ()+this->getScale().getZ()) - cam->getPosition().getZ();
			angleZ = atan2(deltaX, deltaZ) * 180 / D3DX_PI;
			this->setRotation(0, -90, angleZ);
			
		}

		void Update(float deltaTime) {
			frameDuration += deltaTime;
			if (frameDuration > (1.0f / FPS)) {
				frameDuration = 0;
				currentFrame++;
				if (currentFrame == frames.size()) {
					currentFrame = 0;
				}
				Text = frames[currentFrame];
			}
			Mesh::Update(deltaTime);
		};
		void Transform() {
			ImprovedMesh::Transform();
		};

		void LoadTextures(std::string pre, std::string post, int min, int max);

	};
};
