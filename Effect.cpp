#include "Engine.h"

namespace Manbat{
    Effect::Effect()
    {
        p_effect = NULL;
    }

    Effect::~Effect()
    {
        if (p_effect) p_effect->Release();
    }

    bool Effect::Load(std::string filename)
    {
        ID3DXBuffer *errors = 0;
        D3DXCreateEffectFromFile( g_engine->getDevice(), filename.c_str(), 
            0, 0, 0, 0, &p_effect, &errors);
		//D3DXCreateEffectFromFile( g_engine->getDevice(), filename.c_str(), 
        //	0, 0, D3DXSHADER_Debug, 0, &p_effect, &errors);
		//
        if (errors) {
            MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
            return false;
        }
        return true;
    }

    void Effect::setTechnique(std::string technique)
    {
        p_effect->SetTechnique(technique.c_str());
    }


    /**
    The view matrix needs to be updated any time there's a camera view change.
    Effect parameter name: View
    **/
    void Effect::setViewMatrix(D3DXMATRIX matrix, std::string fxViewParam)
    {
        p_effect->SetMatrix( fxViewParam.c_str(), &matrix );
    }

    /**
    The projection matrix does not usually need to be updated after the first time.
    Effect parameter name: Projection
    **/
    void Effect::setProjectionMatrix(D3DXMATRIX matrix, std::string fxProjParam)
    {
        p_effect->SetMatrix( fxProjParam.c_str(), &matrix );
    }

    /**
    The world matrix is the transform for the current mesh being rendered.
    Effect parameter name: World
    **/
    void Effect::setWorldMatrix(D3DXMATRIX matrix, std::string fxWorldParam)
    {
        p_effect->SetMatrix( fxWorldParam.c_str(), &matrix );
    }

    /**
    Begin effect with only one pass assumed for simplicity.
    If more than one pass is used in an effect, obviously this code should be modified.
    For the sake of simplicity I prefer the approach of using just one pass per effect file.
    **/
    bool Effect::Begin()
    {
        if (!p_effect) return false;
        UINT passes;
        p_effect->Begin(&passes, 0);
        if (passes == 0) return false;
        p_effect->BeginPass(0);
        return true;
    }

    /**
    End effect
    **/
    void Effect::End()
    {
        if (!p_effect) return;
        p_effect->EndPass();
        p_effect->End();
    }
        

    void Effect::setParam( std::string name, D3DXMATRIX matrix )
    {
        p_effect->SetMatrix( name.c_str(), &matrix );
    }

    void Effect::setParam( std::string name, LPDIRECT3DTEXTURE9 texture )
    {
        p_effect->SetTexture( name.c_str(), texture );
    }

    void Effect::setParam( std::string name, LPDIRECT3DCUBETEXTURE9 cubeTexture )
    {
        p_effect->SetTexture( name.c_str(), cubeTexture );
    }

    void Effect::setParam( std::string name, D3DXVECTOR4 vector)
    {
        p_effect->SetVector( name.c_str(), &vector );
    }

    void Effect::setParam( std::string name, D3DXVECTOR3 vector )
    {
        D3DXVECTOR4 v;
        v.x = vector.x;
        v.y = vector.y;
        v.z = vector.z;
        v.w = 0;
        p_effect->SetVector( name.c_str(), &v );
    }

    void Effect::setParam( std::string name, Vector3 vector )
    {
        D3DXVECTOR4 v;
        v.x = (float) vector.x;
        v.y = (float) vector.y;
        v.z = (float) vector.z;
        v.w = 0;
        p_effect->SetVector( name.c_str(), &v );
    }

    void Effect::setParam( std::string name, float f )
    {
        p_effect->SetFloat( name.c_str(), f );
    }


};
