#pragma once
#include "Engine.h"

namespace Manbat{
	class MeshLoaderCallback : public ID3DXAllocateHierarchy{
			
	public:
		STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *retNewFrame);
		STDMETHOD(CreateMeshContainer)(
			LPCSTR Name, 
			const D3DXMESHDATA * meshData,
			const D3DXMATERIAL * materials,
			const D3DXEFFECTINSTANCE * effectInstances,
			DWORD numMaterials,
			const DWORD*adjacency,
			LPD3DXSKININFO skinInfo,
			LPD3DXMESHCONTAINER * retNewMeshContainer);
		STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFree);
		STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerToFree);

		bool findFile(std::string *filename);
		bool doesFileExist(std::string &filename);
		void splitPath(std::string* inputPath, std::string* pathOnly, std::string* filenameOnly);

	};
	struct D3DXMESHCONTAINER_NEW: public D3DXMESHCONTAINER{
		IDirect3DTexture9**		textures;
		D3DMATERIAL9*			materials;
		ID3DXMesh*				skinMesh;
		D3DXMATRIX*				boneOffsets;
		D3DXMATRIX**			frameCombinedMatrix;
	};

	struct D3DXFRAME_NEW: public D3DXFRAME{
		D3DXMATRIX combinedMatrix;
	};
	class BoneMesh: public Mesh{
	private:
		LPD3DXFRAME p_frameRoot;
		LPD3DXANIMATIONCONTROLLER p_animController;
		D3DXMESHCONTAINER_NEW* p_firstMesh;

		D3DXMATRIX *p_boneMatrices;
		int p_maxBones;

		unsigned int p_currentAnimationSet;
		unsigned int p_numAnimationSets;
		unsigned int p_currentTrack;

		float p_currentTime;
		float p_speedAdjust;

		int p_vertexCount;
		int p_faceCount;

		std::vector<std::string> p_boneNames;

		void updateFrameMatrices(D3DXFRAME *frameBase, D3DXMATRIX *parentMatrix);
		void drawFrame(LPD3DXFRAME frame , Manbat::Effect* effect);
		void drawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase, Manbat::Effect* effect);
		void createBoneMatrices(D3DXFRAME_NEW *pFrame, LPD3DXMATRIX pParentMatrix);
	public:
		BoneMesh();
		virtual ~BoneMesh(void);
		bool Load(std::string filename);
		void Render(Manbat::Effect* effect);

		void setAnimationSet(unsigned int index);
		unsigned int getCurrentAnimationSet() { return p_currentAnimationSet;}

		int getAnimationSeetIndex(){
				return(p_numAnimationSets > 0 ? p_numAnimationSets:0);
		}

		std::string getAnimationSetName(unsigned int index);

		void Animate(float elapsedTime, D3DXMATRIX *matWorld);

		void nextAnimation();

		void adjustAnimationSpeed(float amount);

		std::string getBoneName(int i);

		std::vector<std::string> getAllBoneNames();

		void buildBoneList(LPD3DXFRAME frame, std::vector<std::string> &names);

		LPD3DXFRAME getRootFrame(){ return p_frameRoot;}

		int getVertexCount(){ return p_vertexCount; }

		int getFaceCount(){ return p_faceCount; }

		int getBoneCount() { return p_maxBones;}
	};
};