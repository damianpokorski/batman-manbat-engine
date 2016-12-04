#pragma once
#include "Engine.h"

namespace Manbat {
	class ImprovedMesh : public Mesh {
		private:
			bool CrashFlag;
		public:
			Texture* Text;
			ImprovedMesh(void);
			~ImprovedMesh() {
				//Mesh::~Mesh();
			};
			bool Disabled;
			bool Init(char* mesh_filename, char* texture_filename = "");
			void LazyRender(Effect* e, Camera* c);
			void LoadTexture(char* texture_filename = "");
			
	};
}