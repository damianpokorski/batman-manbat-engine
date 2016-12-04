#include "Engine.h"

using namespace std;
namespace Manbat {
	ImprovedMesh::ImprovedMesh(void) {
		mesh = 0;
		materials = 0;
		material_count = 0;
		textures = 0;
		position = Vector3(0.0f, 0.0f, 0.0f);

		PreviousPosition = Vector3(0.0f, 0.0f, 0.0f);
		PositionReseted = false;

		rotation = Vector3(0.0f, 0.0f, 0.0f);
		scale = Vector3(1.0f, 1.0f, 1.0f);
		Disabled = false;
	}
	bool ImprovedMesh::Init(char* mesh_filename, char* texture_filename)
	{
		if (strlen(mesh_filename) > 0){
				if (!this->Load(mesh_filename)) {
					Debug << "Error loading Improved mesh \"" << mesh_filename << "\"" << std::endl;
					return false;
				}
		}
		
		if (strlen(texture_filename) > 0) {
			LoadTexture(texture_filename);
		}
		return true;
	}
	void ImprovedMesh::LazyRender(Effect* e, Camera* c) {
		if (Disabled)
			return;
		e->setProjectionMatrix(c->getProjMatrix());
		e->setViewMatrix(c->getViewMatrix());
		e->setWorldMatrix(this->getMatrix());
		e->setParam("Texture", Text->getTexture());
		this->Render(e);
	}

	void ImprovedMesh::LoadTexture(char* texture_filename) {
		Text = g_engine->cacheTexture(texture_filename);
	}
}