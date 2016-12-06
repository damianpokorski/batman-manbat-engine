#pragma once
#include "Engine.h"

namespace Manbat {
	class Scene {
		public:
			Scene();
			std::vector<ImprovedMesh*> contents;
			Vector3 PlayerStart;
			ImprovedCamera* playerController;
			void sortBillboardsByDistance() {
				for (size_t i = 0; i < contents.size(); i++) {
					if (contents[i]->getEntityType() == ENTITY_BILLBOARD) {
						for (size_t j = 0; j < contents.size(); j++) {
							if (contents[j]->getEntityType() == ENTITY_BILLBOARD) {
								if (Math::Distance(playerController->camera->getPosition(), contents[j]->getPosition()) < Math::Distance(playerController->camera->getPosition(), contents[i]->getPosition())) {
									ImprovedMesh* temp = contents[i];
									contents[i] = contents[j];
									contents[j] = temp;
								}
							}
						}
					}
				}
			};
			void Add(char Element, int x, int y);
			void Load(char* filename);
			void LazyRender(Effect* e, Camera* c);
			void Transform();
			void Update(float deltaTime);
	};
};

