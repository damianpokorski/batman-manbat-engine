#pragma once

#include "Engine.h"

namespace Manbat{
    struct Matrix : public D3DXMATRIX
    {
    public:
        Matrix();
        Matrix( const Matrix& );
        Matrix( const D3DXMATRIX& );
        Matrix& operator=( const Matrix& );
        Matrix& operator=( const D3DXMATRIX& );
        void setZero();
        void setIdentity();
        void Fill( int );
        void Fill( float );
        void Fill( double );
        void Translate( float x, float y, float z );
        void Translate( Vector3& );
        void rotateYawPitchRoll( float x, float y, float z );
        void rotateYawPitchRoll( Vector3& );
        void rotateX( float );
        void rotateY( float );
        void rotateZ( float );
        void Scale( float x, float y, float z );
        void Scale( Vector3& );
    };
    
};