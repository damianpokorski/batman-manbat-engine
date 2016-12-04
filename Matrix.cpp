#pragma once
#include "Engine.h"

namespace Manbat{
    Matrix::Matrix() : D3DXMATRIX()
    {
    }

    Matrix::Matrix(const Matrix& M) : D3DXMATRIX(M)
    {
        *this = M;
    }

    Matrix::Matrix( const D3DXMATRIX& M ) : D3DXMATRIX(M)
    {
        *this = M;
    }

    Matrix& Matrix::operator=( const Matrix& M )
    {
        memcpy( m, &M.m, sizeof(float)*16 );
        return *this;
    }

    Matrix& Matrix::operator=( const D3DXMATRIX& M )
    {
        memcpy( m, &M.m, sizeof(float)*16 );
        return *this;
    }

    void Matrix::setZero()
    {
        Fill( 0.0f );
    }

    void Matrix::setIdentity()
    {
        //set most elements to zero
        m[0][1] = m[0][2] = m[0][3] =
        m[1][0] = m[1][2] = m[1][3] =
        m[2][0] = m[2][1] = m[2][3] =
        m[3][0] = m[3][1] = m[3][2] = 0.0f;

        //set diagonals 11,22,33,44 to one
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    }

    void Matrix::Fill( int value )
    {
        Fill( (float) value );
    }

    void Matrix::Fill( float value )
    {
        m[0][0] = m[0][1] = m[0][2] = m[0][3] =
        m[1][0] = m[1][1] = m[1][2] = m[1][3] =
        m[2][0] = m[2][1] = m[2][2] = m[2][3] =
        m[3][0] = m[3][1] = m[3][2] = m[3][3] = value;
    }

    void Matrix::Fill( double value )
    {
        Fill ((float) value );
    }


    void Matrix::Translate( float x, float y, float z )
    {
        D3DXMatrixTranslation( (D3DXMATRIX*) this, x, y, z );
    }

    void Matrix::Translate( Vector3& V )
    {
        Translate( (float)V.x, (float)V.y, (float)V.z );
    }

    void Matrix::rotateYawPitchRoll( float x, float y, float z )
    {
        D3DXMatrixRotationYawPitchRoll( (D3DXMATRIX*) this, x, y, z);
    }

    void Matrix::rotateYawPitchRoll( Vector3& V )
    {
        rotateYawPitchRoll( (float)V.x, (float)V.y, (float)V.z );
    }

    void Matrix::rotateX( float x )
    {
        D3DXMatrixRotationX( (D3DXMATRIX*) this, x );
    }

    void Matrix::rotateY( float y )
    {
        D3DXMatrixRotationY( (D3DXMATRIX*) this, y );
    }

    void Matrix::rotateZ( float z )
    {
        D3DXMatrixRotationX( (D3DXMATRIX*) this, z );
    }


    void Matrix::Scale( float x, float y, float z )
    {
        D3DXMatrixScaling( (D3DXMATRIX*) this, x, y, z );
    }

    void Matrix::Scale( Vector3& V )
    {
        Scale( (float)V.x, (float)V.y, (float)V.z );
    }
}