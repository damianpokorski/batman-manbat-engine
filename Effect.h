#pragma once

#include "Engine.h"

namespace Manbat{

    class Effect
    {
    private:

    public:
		
        LPD3DXEFFECT p_effect;

        Effect();
        ~Effect();
        LPD3DXEFFECT getObject() { return p_effect; }
        bool Load(std::string filename);
        void setTechnique(std::string technique);
        void setViewMatrix(D3DXMATRIX matrix, std::string fxViewParam = "View");
        void setProjectionMatrix(D3DXMATRIX matrix, std::string fxProjParam = "Projection");
        void setWorldMatrix(D3DXMATRIX matrix, std::string fxWorldParam = "World");

        void setParam( std::string name, D3DXMATRIX matrix );
        void setParam( std::string name, LPDIRECT3DTEXTURE9 texture );
        void setParam( std::string name, LPDIRECT3DCUBETEXTURE9 cubeTexture );
        void setParam( std::string name, D3DXVECTOR4 vector);
        void setParam( std::string name, D3DXVECTOR3 vector );
        void setParam( std::string name, Vector3 vector );
        void setParam( std::string name, float f );

        bool Begin();
        void End();

    };

};