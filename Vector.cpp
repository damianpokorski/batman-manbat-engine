#include "Engine.h"

namespace Manbat{
Vector3::Vector3()
{
    x = y = z = 0;
}

Vector3::Vector3( const Vector3& V ) 
{ 
    *this = V; 
}

Vector3::Vector3( double x, double y, double z ) 
{ 
    Set( x, y, z ); 
}

Vector3::Vector3( int x, int y, int z)
{ 
    Set((double)x,(double)y,(double)z); 
}

Vector3::Vector3(const D3DXVECTOR2& dv)
{
    x=dv.x; y=dv.y; z=0.0;
}

Vector3::Vector3(const D3DXVECTOR3& dv)
{
    x=dv.x; y=dv.y; z=dv.z;
}

//assignment operator
Vector3& Vector3::operator=( const Vector3& V) 
{
    Set(V);
    return *this;
}

void Vector3::Set( double x1,double y1,double z1 ) 
{ 
    x=x1; y=y1; z=z1; 
}

void Vector3::Set( const Vector3& V) 
{ 
    x=V.x; y=V.y; z=V.z; 
}

void Vector3::Move( double mx,double my,double mz) 
{ 
    x+=mx; y+=my; z+=mz; 
}

void Vector3::operator+=(const Vector3& V) 
{ 
    x+=V.x; y+=V.y; z+=V.z; 
}

void Vector3::operator-=(const Vector3& V) 
{ 
    x-=V.x; y-=V.y; z-=V.z; 
}

void Vector3::operator*=(const Vector3& V) 
{ 
    x*=V.x; y*=V.y; z*=V.z; 
}

void Vector3::operator/=(const Vector3& V) 
{ 
    x/=V.x; y/=V.y; z/=V.z; 
}

Vector3 Vector3::operator/(const double& d)
{
    Vector3 v( x/d, y/d, z/d);
    return v;
}

Vector3 Vector3::operator*(const double& d)
{
    Vector3 v( x*d, y*d, z*d);
    return v;
}
Vector3 Vector3::operator+(const double& d)
{
    Vector3 v( x+d, y+d, z+d);
    return v;
}
Vector3 Vector3::operator-(const double& d)
{
    Vector3 v( x-d, y-d, z-d);
    return v;
}
Vector3 Vector3::operator/(const Vector3& d)
{
    Vector3 v( x/d.x, y/d.y, z/d.z);
    return v;
}

Vector3 Vector3::operator*(const Vector3& d)
{
    Vector3 v( x*d.x, y*d.y, z*d.z);
    return v;
}
Vector3 Vector3::operator+(const Vector3& d)
{
    Vector3 v( x+d.x, y+d.y, z+d.z);
    return v;
}
Vector3 Vector3::operator-(const Vector3& d)
{
    Vector3 v( x-d.x, y-d.y, z-d.z);
    return v;
}

//equality operator comparison includes double rounding
bool Vector3::operator==( const Vector3& V ) const
{ 
    return (
        (((V.x - 0.0001f) < x) && (x < (V.x + 0.0001f))) &&
        (((V.y - 0.0001f) < y) && (y < (V.y + 0.0001f))) &&
        (((V.z - 0.0001f) < z) && (z < (V.z + 0.0001f))) );
}

//inequality operator
bool Vector3::operator!=( const Vector3& V ) const 
{
    return (!(*this == V));
}

D3DXVECTOR3 Vector3::ToD3DXVECTOR3()
{
    return D3DXVECTOR3( (float)x, (float)y, (float)z);
}

D3DXVECTOR2 Vector3::ToD3DXVECTOR2()
{
    return D3DXVECTOR2( (float)x, (float)y );
}


    /**
    Vector2 class implementation
    **/
    Vector2::Vector2()
    {
        x = y = 0;
    }
    
    Vector2::Vector2( const Vector2& V ) 
    { 
        *this = V; 
    }
    
    Vector2::Vector2( double x, double y ) 
    { 
        Set( x, y ); 
    }
    
    Vector2::Vector2( int x, int y )
    { 
        Set((double)x,(double)y); 
    }

    Vector2::Vector2(const D3DXVECTOR2& dv)
    {
        x=dv.x; y=dv.y;
    }

    Vector2::Vector2(const D3DXVECTOR3& dv)
    {
        x=dv.x; y=dv.y;
    }

    Vector2& Vector2::operator=( const Vector2& V) 
    {
        Set( V );
        return *this;
    }


    void Vector2::Set( double x1,double y1 ) 
    { 
        x=x1; y=y1;
    }
    
    void Vector2::Set( const Vector2& V) 
    { 
        x=V.x; y=V.y;
    }
    
    void Vector2::Move( double mx,double my ) 
    { 
        x+=mx; y+=my; 
    }
    
    void Vector2::operator+=(const Vector2& V) 
    { 
        x+=V.x; y+=V.y; 
    }
    
    void Vector2::operator-=(const Vector2& V) 
    { 
        x-=V.x; y-=V.y; 
    }
    
    void Vector2::operator*=(const Vector2& V) 
    { 
        x*=V.x; y*=V.y; 
    }
    
    void Vector2::operator/=(const Vector2& V) 
    { 
        x/=V.x; y/=V.y; 
    }

    Vector2 Vector2::operator/(const double& d)
    {
        Vector2 v( x/d, y/d );
        return v;
    }
     Vector2 Vector2::operator*(const double& d)
    {
        Vector2 v( x*d, y*d );
        return v;
    }
	   Vector2 Vector2::operator+(const double& d)
    {
        Vector2 v( x+d, y+d );
        return v;
    }
     Vector2 Vector2::operator-(const double& d)
    {
        Vector2 v( x-d, y-d );
        return v;
    }
    //equality operator comparison includes double rounding
    bool Vector2::operator==( const Vector2& V ) const
    { 
        return (
            (((V.x - 0.0001f) < x) && (x < (V.x + 0.0001f))) &&
            (((V.y - 0.0001f) < y) && (y < (V.y + 0.0001f))) );
    }
    
    //inequality operator
    bool Vector2::operator!=( const Vector2& V ) const 
    {
        return (!(*this == V));
    }
    
    D3DXVECTOR3 Vector2::ToD3DXVECTOR3()
    {
        return D3DXVECTOR3( (float)x, (float)y, 0.0f );
    }

    D3DXVECTOR2 Vector2::ToD3DXVECTOR2()
    {
        return D3DXVECTOR2( (float)x, (float)y );
    }
}