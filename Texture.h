#pragma once

#include "Engine.h"

namespace Manbat{
    class Texture
    {
    private:
        LPDIRECT3DSURFACE9 renderDepthStencilSurface;
        LPDIRECT3DSURFACE9 renderSurface;

    public:
        Texture();
        ~Texture();
        bool Create(int width,int height);
        bool Load(std::string filename, Color transcolor = Color(255,0,255,0));
        bool Save(std::string filename, _D3DXIMAGE_FILEFORMAT format = D3DXIFF_BMP);
        void Release();

        bool createRenderTarget(int width,int height);
        bool renderStart(bool clear = true, bool sprite = true, Color clearColor = Color(255,0,255,0));
        bool renderStop(bool sprite = true);

        LPDIRECT3DTEXTURE9 getTexture() { return texture; }
        int getWidth();
        int getHeight();
        RECT getBounds();

        LPDIRECT3DTEXTURE9 texture;
        D3DXIMAGE_INFO info;
    };

};