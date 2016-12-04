#pragma once
#include "Engine.h"

namespace Manbat{
    double Math::toDegrees(double radians)
    {
        return radians * PI_under_180;
    }
    
    double Math::toRadians(double degrees)
    {
        return degrees * PI_over_180;
    }
    
    double Math::wrapAngleDegs(double degs)
    {
        double result = fmod(degs, 360.0);
        if (result < 0) result += 360.0f;
        return result;
    }
    
    double Math::wrapAngleRads(double rads)
    {
        double result = fmod(rads, PI);
        if (result < 0) result += PI;
        return result;
    }

    double Math::wrapValue(double value, double min, double max)
    {
        if (value < min) value = max;
        else if (value > max) value = min;
        return value;
    }

    double Math::Limit(double value, double min, double max)
    {
        if (value < min) value = min;
        else if (value > max) value = max;
        return value;
    }

    /**
    Calculate X velocity based on degree angle
    **/
    double Math::linearVelocityX(double angle)
    {
        angle -= 90;
        if (angle < 0) angle = 360 + angle;    
        return cos( toRadians( angle ) );
    }
    
    /**
    Calculate Y velocity based on degree angle
    **/
    double Math::linearVelocityY(double angle)
    {
        angle -= 90;
        if (angle < 0) angle = 360 + angle;    
        return sin( toRadians( angle ) );
    }
    
    /**
    Calculate Vector velocity based on degree angle
    **/
    Vector2 Math::linearVelocity(double angle)
    {
        double vx = linearVelocityX(angle);
        double vy = linearVelocityY(angle);
        return Vector2(vx,vy);
    }

    double Math::angleToTarget(double x1,double y1,double x2,double y2)
    {
        double deltaX = (x2-x1);
        double deltaY = (y2-y1);
        return atan2(deltaY,deltaX);
    }
    
    double Math::angleToTarget(Vector3& A, Vector3& B)
    {
        return angleToTarget(A.getX(),A.getY(),B.getX(),B.getY());
    }

    double Math::angleToTarget(Vector2& A, Vector2& B)
    {
        return angleToTarget(A.getX(),A.getY(),B.getX(),B.getY());
    }

    double Math::Distance( double x1,double y1,double z1, double x2,double y2,double z2 )
    {
        double deltaX = (x2-x1);
        double deltaY = (y2-y1);
        double deltaZ = (z2-z1);
        return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
    }

    double Math::Distance( double x1,double y1,double x2,double y2 )
    {
        double deltaX = (x2-x1);
        double deltaY = (y2-y1);
        return sqrt(deltaX*deltaX + deltaY*deltaY);
    }

    double Math::Distance( Vector3& A, Vector3& B ) 
    {
        return Distance(A.getX(),A.getY(),A.getZ(), B.getX(),B.getY(),B.getZ());
    }

    double Math::Distance( Vector2& A, Vector2& B ) 
    {
        return Distance(A.getX(),A.getY(), B.getX(),B.getY());
    }

    double Math::Length(double x,double y,double z)
    {
        return sqrt(x*x + y*y + z*z);
    }
    
    double Math::Length(double x,double y)
    {
        return sqrt(x*x + y*y);
    }

    double Math::Length(Vector3& V)
    {
        return Length(V.getX(),V.getY(),V.getZ());
    }

    double Math::Length(Vector2& V)
    {
        return Length(V.getX(),V.getY());
    }

    double Math::dotProduct(double x1,double y1,double z1,double x2,double y2,double z2)
    {
        return (x1*x2 + y1*y2 + z1*z2);
    }
    
    double Math::dotProduct(double x1,double y1,double x2,double y2)
    {
        return (x1*x2 + y1*y2);
    }

    double Math::dotProduct( Vector3& A, Vector3& B )
    {
        return dotProduct(A.getX(),A.getY(),A.getZ(),B.getX(),B.getY(),B.getZ());
    }
    
    double Math::dotProduct( Vector2& A, Vector2& B )
    {
        return dotProduct(A.getX(),A.getY(),B.getX(),B.getY());
    }
    
    Vector3 Math::crossProduct( double x1,double y1,double z1,double x2,double y2,double z2)
    {
        double nx = (y1*z2)-(z1*y2);
        double ny = (z1*y2)-(x1*z2);
        double nz = (x1*y2)-(y1*x2);
        return Vector3(nx,ny,nz);
    }

    Vector3 Math::crossProduct( Vector3& A, Vector3& B )
    {
        return crossProduct(A.getX(),A.getY(),A.getZ(),B.getX(),B.getY(),B.getZ());
    }
    
    Vector3 Math::Normal(double x,double y,double z)
    {
        /*double length = Length(x,y,z);
        if (length != 0) length = 1 / length;
        double nx = x*length;
        double ny = y*length;
        double nz = z*length;
        return Vector3(nx,ny,nz);
		*/
		double length = Length(x,y,z);
        if (length == 0) return Vector3(0,0,0);
        double nx = x/length;
        double ny = y/length;
        double nz = z/length;
        return Vector3(nx,ny,nz);
    }
    
    Vector3 Math::Normal(Vector3& V)
    {
        return Normal(V.getX(),V.getY(),V.getZ());
    }
}