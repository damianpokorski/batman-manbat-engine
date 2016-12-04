#pragma once
#include "Engine.h"

namespace Manbat{
	struct BBOX{
		Vector3 min;
		Vector3 max;
	};
	struct BSPHERE{
		Vector3 center;
		float radius;
	};
	class Mesh: public Entity{
		private:
			
		public:

			LPD3DXMESH mesh;
			D3DMATERIAL9* materials;
			DWORD material_count;
			LPDIRECT3DTEXTURE9* textures;

			Vector3 position;
			Vector3 rotation;
			Vector3 scale;

			Matrix matWorld;
			Matrix matTranslate;
			Matrix matRotate;
			Matrix matScale;

			Effect *p_effect;

			Mesh(void);
			virtual ~Mesh(void);

			bool Load(char* filename, bool comuteNormals=false);

			bool findFile(std::string *filename);
			bool doesFileExist(std::string &filename);
			void splitPath(std::string& inputPath, std::string* pathOnly, std::string* filenameOnly);

			void Update(float deltaTime);
			void Render();
			void Render(Effect *effect, std::string fxTextureParam="Texture");

			void setEffect(Effect* effect){ p_effect = effect;};

			Matrix getMatrix(){return matWorld;}
			void setMatrix(Matrix matrix){matWorld=matrix;};
			void setmatrix(D3DXMATRIX matrix){matWorld = (Matrix)matrix;};

			LPD3DXMESH getMesh(){return mesh;};
			virtual int getFaceCount();
			virtual int getVertexCount();

			void createTorus(float innerRadius = 0.0f, float outerRadius = 1.0f, int sides = 20, int rings = 20);
			void createCylinder(float radius1, float radius2, float length, int slices, int stacks);
			void createSphere(double radius = 1.0f, int slices = 1.0f, int stacks = 10);
			void createCube(double width = 1.0f, double height = 1.0f, double depth = 1.0f);
			void createTeapot();

			void Transform();

			void Rotate(Vector3 rot);
			void Rotate(double x, double y, double xz);

			void setPosition(double x, double y, double z);
			Vector3 getPosition(){ return position;};
			void setRotation(Vector3 rot) { rotation = rot;};
			void setRotation(double x, double y, double z);
			Vector3 getRotation(){ return rotation; };
			void setScale(Vector3 value){ scale = value;};
			void setScale(double x, double y, double z);
			void setScale(double value);
			Vector3 getScale(){return scale;};
			BBOX getBoundingBox();
			BSPHERE getBoundingSphere();

			// My stuff

			Vector3 PreviousPosition;
			bool PositionReseted;

			void RevertPosition() { position = PreviousPosition; PositionReseted = true; };
			void setPosition(Vector3 pos) {
				// My Test
				PreviousPosition = position;
				// My Test
				position = pos;
			};

			bool grounded;

			Vector3 pivot;
			void setPivot(Vector3 p) { pivot = p; };
			Vector3 getPivot() { return pivot; };
			
			BYTE* pVerticesCache;
			BBOX cachedBoundingBox;

			// Googled function
			LPD3DXMESH CopyMesh(LPD3DXMESH srcMesh)
			{
				// make sure mesh is 
				if (srcMesh == NULL)
					return NULL;

				LPDIRECT3DDEVICE9 pTempDevice = NULL;

				// get the device
				if (FAILED(srcMesh->GetDevice(&pTempDevice)))
					return NULL;

				// the new mesh to be created
				LPD3DXMESH newMesh = NULL;

				HRESULT hr;

				// clone the mesh
				hr = srcMesh->CloneMeshFVF(srcMesh->GetOptions(), srcMesh->GetFVF(), pTempDevice, &newMesh);

				// release the device
				pTempDevice->Release();

				// see if mesh was cloned
				if (FAILED(hr))
					return NULL;

				// success
				return newMesh;
			}
	};
};