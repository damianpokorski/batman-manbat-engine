#include "Engine.h"
using namespace std;
using namespace Manbat;
namespace Manbat {
	class ImprovedSkybox
	{
	public:
		ImprovedSkybox() {};
		Effect* shader;
		Skybox* image;
		void Init(std::string SkyboxEffectFile, std::string SkyboxImage) {
			// Loading skybox shaders
			shader = new Effect();
			if (!shader->Load(SkyboxEffectFile)) {
				Debug << "Error loading skybox effect";
			}

			image = new Skybox();
			if (!image->Create(SkyboxImage)) {
				Debug << "Error loading skybox";
			}
		};
		void RenderSkybox(Camera* c) {
			Matrix matWorld;
			matWorld.setIdentity();
			shader->setWorldMatrix(matWorld);
			shader->setViewMatrix(c->getViewMatrix());
			shader->setProjectionMatrix(c->getProjMatrix());

			shader->Begin();
			image->Render();
			shader->End();

		}
	};
}