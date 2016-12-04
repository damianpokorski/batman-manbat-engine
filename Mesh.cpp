#include "Engine.h"

using namespace std;
namespace Manbat{

	Mesh::Mesh() :Entity(RENDER_3D)
	{
		mesh = 0;
		materials = 0;
		material_count = 0;
		textures = 0;
		position = Vector3(0.0f,0.0f,0.0f);
		// My stuff
		PreviousPosition = Vector3(0.0f,0.0f,0.0f);
		PositionReseted = false;
		rotation = Vector3(0.0f,0.0f,0.0f);
		scale = Vector3(1.0f,1.0f,1.0f);
		grounded = false;
		pVerticesCache = NULL;
		pivot = Vector3(0, 0, 0);
	}

	Mesh::~Mesh(void)
	{
		return;
		if (materials != NULL) delete[] materials;

		//remove textures from memory
		if (textures != NULL) {
			for( DWORD i = 0; i < material_count; i++)
			{
				if (textures[i] != NULL)
					textures[i]->Release();
			}
			delete[] textures;
		}
		
		if (mesh != NULL) mesh->Release();
	}

	int Mesh::getFaceCount()
	{
		return mesh->GetNumFaces();
	}

	int Mesh::getVertexCount()
	{
		return mesh->GetNumVertices();
	}

	void Mesh::createTorus(float innerRadius, float outerRadius, int sides, int rings)
	{
		D3DXCreateTorus(g_engine->getDevice(), innerRadius, outerRadius, sides, rings, &mesh, NULL);
		
	}

	void Mesh::createCylinder(float radius1, float radius2, float length, int slices, int stacks)
	{
		D3DXCreateCylinder(g_engine->getDevice(), radius1, radius2, length, slices, stacks, &mesh, NULL);
	}

	void Mesh::createSphere(double radius, int slices, int stacks)
	{
		D3DXCreateSphere(g_engine->getDevice(), (float)radius, slices, stacks, &mesh, NULL);
	}

	void Mesh::createCube(double width, double height, double depth)
	{
		D3DXCreateBox(g_engine->getDevice(), (float)width, (float)height, (float)depth, &mesh, NULL);
	}

	void Mesh::createTeapot()
	{
		D3DXCreateTeapot(g_engine->getDevice(), &mesh, NULL);
	}


	void Mesh::splitPath(string& inputPath, string* pathOnly, string* filenameOnly)
	{
		string fullPath( inputPath );
		replace( fullPath.begin(), fullPath.end(), '\\', '/');
		string::size_type lastSlashPos = fullPath.find_last_of('/');

		// check for there being no path element in the input
		if (lastSlashPos == string::npos)
		{
			*pathOnly="";
			*filenameOnly = fullPath;
		}
		else {
			if (pathOnly) {
				*pathOnly = fullPath.substr(0, lastSlashPos);
			}
			if (filenameOnly)
			{
				*filenameOnly = fullPath.substr( 
					lastSlashPos + 1,
					fullPath.size() - lastSlashPos - 1 );
			}
		} 
	}

	bool Mesh::doesFileExist(string &filename)
	{
		return (_access(filename.c_str(), 0) != -1);
	}

	bool Mesh::findFile(string *filename)
	{
		if (!filename) return false;

		//since the file was not found, try removing the path
		string pathOnly;
		string filenameOnly;
		splitPath(*filename,&pathOnly,&filenameOnly);

		//is file found in current folder, without the path?
		if (doesFileExist(filenameOnly))
		{
			*filename=filenameOnly;
			return true;
		}

		//not found
		return false;
	}    

	bool Mesh::Load(char* filename, bool computeNormals)
	{
		HRESULT result;
		LPD3DXBUFFER matbuffer = NULL;
		LPD3DXMATERIAL d3dxMaterials;
		if (g_engine->MeshExists(filename)) {
			mesh = CopyMesh(g_engine->MeshCache[filename].Mesh);
			material_count = g_engine->MeshCache[filename].material_count;
			materials = g_engine->MeshCache[filename].materials;
			textures = g_engine->MeshCache[filename].textures;
			Debug << "[Mesh] Loading mesh '" << filename << "' from cache" << std::endl;
		}
		else {
			Debug << "[Mesh] Loading mesh from file: " << filename << std::endl;
			//load mesh from the specified file
			result = D3DXLoadMeshFromX(
				filename,               //filename
				D3DXMESH_SYSTEMMEM,     //mesh options
				g_engine->getDevice(),  //Direct3D device
				NULL,                   //adjacency buffer
				&matbuffer,             //material buffer
				NULL,                   //special effects
				&material_count,         //number of materials
				&mesh);                  //resulting mesh

			if (result != D3D_OK) {
				return false;
			}
		}
		if (!g_engine->MeshExists(filename)) {
			//extract material properties and texture names from material buffer
			d3dxMaterials = (LPD3DXMATERIAL)matbuffer->GetBufferPointer();
			materials = new D3DMATERIAL9[material_count];
			textures = new LPDIRECT3DTEXTURE9[material_count];

			//create the materials and textures
			for (DWORD i = 0; i < material_count; i++)
			{
				//grab the material
				materials[i] = d3dxMaterials[i].MatD3D;
				//load texture(s)
				textures[i] = NULL;
				
				if (d3dxMaterials[i].pTextureFilename != NULL)
				{
					string filename = d3dxMaterials[i].pTextureFilename;
					
					if (findFile(&filename))
					{
						if (g_engine->TextureExists(filename)) {
							textures[i] = g_engine->TextureCache[filename].Texture;
							Debug << "[Mesh] Texture '" << filename << "' loaded from cache." << std::endl;
						}
						else {
							//load texture file specified in .x file
							result = D3DXCreateTextureFromFile(g_engine->getDevice(), filename.c_str(), &textures[i]);
							if (result != D3D_OK) return false;
							//verify texture header
							if (textures[i]->GetType() != D3DRTYPE_TEXTURE) return false;
							//
							g_engine->TextureCache[filename].Texture = textures[i];
							Debug << "[Mesh] Texture '" << filename << "' loaded from file." << std::endl;
						}
					}
					else {
						Debug << "[Mesh] Texture: " << filename << "does not exists" << std::endl;
					}
				}
			}

			//done using material buffer
			matbuffer->Release();
			if (computeNormals)
			{
				HRESULT res = D3DXComputeNormals(mesh, NULL);
				if (res != S_OK)
				{
					Debug << "[Mesh] Mesh Create Sphere: Error computing normals\n";
				}
			}
		}
		// Mesh
		g_engine->MeshCache[filename].Mesh = CopyMesh(mesh);
		// Material count
		g_engine->MeshCache[filename].material_count = material_count;
		// Materials
		g_engine->MeshCache[filename].textures = textures;
		g_engine->MeshCache[filename].materials = materials;
		return true;
	}

	void Mesh::Update(float deltaTime)
	{
		Transform();
	}

	void Mesh::Render()
	{
		Render( p_effect, "Texture" );
	}

	void Mesh::Render( Effect *effect, std::string fxTextureParam )
	{
		p_effect = effect;
		p_effect->setWorldMatrix( matWorld );
		p_effect->Begin();

		if (material_count == 0) 
		{
			mesh->DrawSubset(0);
		}
		else {
			//draw each mesh subset
			for( DWORD i=0; i < material_count; i++ )
			{
				// set the texture used by this face
				if (textures[i]) 
				{
					p_effect->setParam( fxTextureParam, (textures[i]) );
				}
				
				// Draw the mesh subset
				mesh->DrawSubset( i );
			}
		}

		p_effect->End();
	}


	void Mesh::Transform()
	{
		//set rotation matrix
		double x = D3DXToRadian(rotation.x);
		double y = D3DXToRadian(rotation.y);
		double z = D3DXToRadian(rotation.z);
		D3DXMatrixRotationYawPitchRoll(&matRotate, (float)x, (float)y, (float)z);

		//set scaling matrix
		D3DXMatrixScaling(&matScale, (float)scale.x, (float)scale.y, (float)scale.z);

		//set translation matrix
		D3DXMatrixTranslation(&matTranslate, (float)position.x, (float)position.y, (float)position.z);
		
		//transform the mesh
		// matWorld = matRotate * matScale * matTranslate;
		// Transform with pivot
		Matrix PivotPre;
		Matrix PivotAfter;
		D3DXMatrixTranslation(&PivotPre, (float)-getPivot().getX(), (float)-getPivot().getY(), (float)-getPivot().getZ());
		D3DXMatrixTranslation(&PivotAfter, (float)getPivot().getX(), (float)getPivot().getY(), (float)getPivot().getZ());
		matWorld = PivotPre * matRotate * PivotAfter * matScale * matTranslate;
	}

	void Mesh::setPosition(double x,double y,double z) 
	{ 
		position = Vector3((float)x,(float)y,(float)z); 
	}

	void Mesh::Rotate(Vector3 rot)
	{
		Rotate(rot.x,rot.y,rot.z);
	}

	void Mesh::Rotate(double x,double y,double z)
	{
		rotation.x += (float)x;
		rotation.y += (float)y;
		rotation.z += (float)z;
	}

	void Mesh::setRotation(double x,double y,double z) 
	{ 
		rotation = Vector3((float)x,(float)y,(float)z); 
	}


	void Mesh::setScale(double x,double y,double z) 
	{
		scale = Vector3((float)x,(float)y,(float)z); 
	}

	void Mesh::setScale(double value)
	{
		scale.x = scale.y = scale.z = value;
	}

	BSPHERE Mesh::getBoundingSphere(){
		BYTE* pVertices = NULL;
		mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);

		D3DXVECTOR3 center;
		float radius;
		D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)pVertices,
			mesh->GetNumVertices(),
			D3DXGetFVFVertexSize(mesh->GetFVF()),
			&center,
			&radius);

		mesh->UnlockVertexBuffer();

		BSPHERE sphere;
		sphere.center = Vector3(center);
		radius  = radius;
		return sphere;
	}
	BBOX Mesh::getBoundingBox(){
		//BYTE* pVertices = NULL;
		D3DXVECTOR3 minbounds, maxbounds;
		if (pVerticesCache != NULL) {
			D3DXComputeBoundingBox(
				(D3DXVECTOR3*)pVerticesCache,
				mesh->GetNumVertices(),
				D3DXGetFVFVertexSize(mesh->GetFVF()),
				&minbounds,
				&maxbounds
				);

			mesh->UnlockVertexBuffer();
		}
		else {
			// Set the cache up - and loop back to the start of function, cascade the returns
			mesh->LockVertexBuffer(0, (LPVOID*)&pVerticesCache);
			return getBoundingBox();
		}

		BBOX box;
		Vector3 minimum = minbounds;
		Vector3 maximum = maxbounds;
		
		box.min.x = minbounds.x * scale.x;
		box.min.y = minbounds.y * scale.y;
		box.min.z = minbounds.z * scale.z;
		box.max.x = maxbounds.x * scale.x;
		box.max.y = maxbounds.y * scale.y;
		box.max.z = maxbounds.z * scale.z;
			 
		return box;
	}
}