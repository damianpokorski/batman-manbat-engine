#pragma once
#include "Engine.h"

namespace Manbat
{
    class Vector3;
    class Vector2;

    /**
    This define lets us use Point interchangeably with Vector2
    **/
    #define Point Vector2

    /**
    Vector3 (X,Y,Z) class interface
    **/
    class Vector3 
    {
    public:
        double x, y, z;

        virtual ~Vector3() {}
        Vector3();
        Vector3(const Vector3& V);
        Vector3(double x, double y, double z);
        Vector3(int x, int y, int z);
        Vector3(const D3DXVECTOR2& dv);
        Vector3(const D3DXVECTOR3& dv);
        Vector3& operator=( const Vector3& V);

        //manipulation functions
        void Set(double x1,double y1,double z1);
        void Set(const Vector3& V);
        double getX() { return x; }
        void setX(double value) { x = value; }
        double getY() { return y; }
        void setY(double value) { y = value; }
        double getZ() { return z; }
        void setZ(double value) { z = value; }
        void Move( double mx,double my,double mz);
        void operator+=(const Vector3& V);
        void operator-=(const Vector3& V);
        void operator*=(const Vector3& V);
        void operator/=(const Vector3& V);
        Vector3 operator/(const double& d);
		Vector3 operator*(const double& d);
		Vector3 operator+(const double& d);
		Vector3 operator-(const double& d);
		Vector3 operator/(const Vector3& d);
		Vector3 operator*(const Vector3& d);
		Vector3 operator+(const Vector3& d);
		Vector3 operator-(const Vector3& d);
        bool operator==( const Vector3& V ) const;
        bool operator!=( const Vector3& V ) const;

        //exporters to Direct3D vectors
        D3DXVECTOR3 ToD3DXVECTOR3();
        D3DXVECTOR2 ToD3DXVECTOR2();
		// To string for debug purposes
		std::string ToString(){
			std::stringstream ss;
			ss << "X:" << getX() << ", Y: " << getY() << ", Z: " << getZ();
			return ss.str();
		};
    }; 

    /**
    Vector2 (X,Y) class interface
    **/
    class Vector2
    {
    public:
        double x, y;

        virtual ~Vector2() {}
        Vector2();
        Vector2(const Vector2& V);
        Vector2(double x, double y);
        Vector2(int x, int y);
        Vector2(const D3DXVECTOR2& dv);
        Vector2(const D3DXVECTOR3& dv);
        Vector2& operator=( const Vector2& V);

        //manipulation functions
        void Set(double x1,double y1);
        void Set(const Vector2& V);
        double getX() { return x; }
        void setX(double value) { x = value; }
        double getY() { return y; }
        void setY(double value) { y = value; }
        void Move( double mx,double my );
        void operator+=(const Vector2& V);
        void operator-=(const Vector2& V);
        void operator*=(const Vector2& V);
        void operator/=(const Vector2& V);
        Vector2 operator/(const double& d);
		Vector2 operator*(const double& d);
		Vector2 operator+(const double& d);
		Vector2 operator-(const double& d);
        bool operator==( const Vector2& V ) const;
        bool operator!=( const Vector2& V ) const;

        //exporters to Direct3D vectors
        D3DXVECTOR3 ToD3DXVECTOR3();
        D3DXVECTOR2 ToD3DXVECTOR2();
    }; 
};