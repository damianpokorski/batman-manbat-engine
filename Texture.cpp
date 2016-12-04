#include "Engine.h"

using namespace Manbat;

Texture::Texture()
{
	texture = NULL;
	renderDepthStencilSurface = NULL;
	renderSurface = NULL;
}

Texture::~Texture()
{
	Release();
}

int Texture::getWidth() 
{ 
	return info.Width; 
};

int Texture::getHeight() 
{ 
	return info.Height; 
};

RECT Texture::getBounds()
{
	RECT rect = {0, 0, getWidth()-1, getHeight()-1};
	return rect;
}

bool Texture::Load(std::string filename, Color transcolor)
{
	//standard Windows return value
	HRESULT result;
	
	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK)     {
		texture = NULL;
		return false;
	}

	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
		g_engine->getDevice(), //Direct3D device object
		filename.c_str(),      //bitmap filename
		info.Width,            //bitmap image width
		info.Height,           //bitmap image height
		1,                     //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       //the type of surface (standard)
		D3DFMT_UNKNOWN,        //surface format (default)
		D3DPOOL_DEFAULT,       //memory class for the texture
		D3DX_DEFAULT,          //image filter
		D3DX_DEFAULT,          //mip filter
		transcolor.ToD3DCOLOR(),            //color key for transparency
		&info,                 //bitmap file info (from loaded file)
		NULL,                  //color palette
		&texture );            //destination texture

	//make sure the bitmap textre was loaded correctly
	if (result != D3D_OK)     {
		texture = NULL;
		return false;
	}

	return true;
}

void Texture::Release()
{
	if (texture) texture->Release();
	if (renderDepthStencilSurface) renderDepthStencilSurface->Release();
	if (renderSurface) renderSurface->Release();
}



bool Texture::Create(int width,int height)
{
	//if texture is already in use, first deallocate memory
	if (texture)
	{
		texture->Release();
		texture = NULL;
	}

	HRESULT r;
	r = D3DXCreateTexture(g_engine->getDevice(), width, height, 1, 
		D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture );
	if (r != D3D_OK) {
		texture = NULL;
		return false;
	}

	//save texture info
	info.Width = width;
	info.Height = height;
	info.Format = D3DFMT_A8R8G8B8;

	return true;
}

bool Texture::createRenderTarget(int width,int height)
{
	//if texture is already in use, first deallocate memory
	if (texture)
	{
		texture->Release();
		texture = NULL;
	}

	//create the render target surface, depth stencil
	g_engine->getDevice()->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL );
	g_engine->getDevice()->CreateDepthStencilSurface( width, height, D3DFMT_D16, 
		D3DMULTISAMPLE_NONE, 0, false, &renderDepthStencilSurface, NULL );
	texture->GetSurfaceLevel( 0, &renderSurface );

	//save texture info
	info.Width = width;
	info.Height = height;
	info.Format = D3DFMT_A8R8G8B8;

	return true;
}

/**
	Used only when texture is a render target
	and never when the primary device is rendering!
**/
bool Texture::renderStart(bool clear, bool sprite, Color clearColor)
{
	g_engine->getDevice()->SetRenderTarget( 0, renderSurface );
	g_engine->getDevice()->SetDepthStencilSurface( renderDepthStencilSurface );
	if (clear) 
	{
		g_engine->getDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, 
			clearColor.ToD3DCOLOR(), 1.0f, 0 );
	}
	g_engine->getDevice()->BeginScene();

	if (sprite)
		g_engine->getSpriteObject()->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	g_engine->getSpriteObject()->SetTransform(&identity);

	return true;
}

/**
	Used only when texture is a render target
	and never when the primary device is rendering!
**/
bool Texture::renderStop(bool sprite)
{
	if (sprite)
		g_engine->getSpriteObject()->End();

	g_engine->getDevice()->EndScene();
	return true;
}


/**
	Saves a texture in memory to a file. 
	Supports TGA, JPG, PNG, DDS, BMP (default).
**/
bool Texture::Save(std::string filename, _D3DXIMAGE_FILEFORMAT format)
{
	if (texture)
	{
		D3DXSaveTextureToFile(filename.c_str(), format, texture, NULL);
		return true;
	}
	else
		return false;
}

