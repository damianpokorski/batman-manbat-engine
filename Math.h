#pragma once

#include "Engine.h"

namespace Manbat{
    const double PI = 3.1415926535;
    const double PI_over_180 = PI / 180.0f;
    const double PI_under_180 = 180.0f / PI;
    
    class Math 
    {
    public:
        static double toDegrees(double radian);
        static double toRadians(double degree);
        static double wrapAngleDegs(double degs);
        static double wrapAngleRads(double rads);
        static double wrapValue(double value, double min, double max);
        static double Limit(double value, double min, double max); //***addition
        static double linearVelocityX(double angle);
        static double linearVelocityY(double angle);
        static Vector2 linearVelocity(double angle);
        static double angleToTarget(double x1,double y1,double x2,double y2);
        static double angleToTarget(Vector3& source,Vector3& target);
        static double angleToTarget(Vector2& source,Vector2& target);
        static double Distance(double x1,double y1,double z1, double x2,double y2,double z2);
        static double Distance(double x1,double y1,double x2,double y2);
        static double Distance(Vector3& A, Vector3& B);
        static double Distance(Vector2& A, Vector2& B);
        static double Length(double x,double y,double z);
        static double Length(double x,double y);
        static double Length(Vector3& V);
        static double Length(Vector2& V);
        static double dotProduct(double x1,double y1,double z1,double x2,double y2,double z2);
        static double dotProduct(double x1,double y1,double x2,double y2);
        static double dotProduct(Vector3& A, Vector3& B);
        static double dotProduct(Vector2& A, Vector2& B);
        static Vector3 crossProduct(double x1,double y1,double z1,double x2,double y2,double z2);
        static Vector3 crossProduct(Vector3& A, Vector3& B);
        static Vector3 Normal(double x,double y,double z);
        static Vector3 Normal(Vector3& V);
		// Extra bits
		static float Approach(float flGoal, float flCurrent, float dt) {
			float flDifference = flGoal - flCurrent;

			if (flDifference > dt)
				return flCurrent + dt;
			if (flDifference < -dt)
				return flCurrent - dt;

			return flGoal;
		};
		static Vector3 Approach(Vector3 flGoal, Vector3 flCurrent, float dt) {
			Vector3 result = Vector3(0, 0, 0);
			result.setX(Approach(flGoal.getX(), flCurrent.getX(), dt));
			result.setY(Approach(flGoal.getY(), flCurrent.getY(), dt));
			result.setZ(Approach(flGoal.getZ(), flCurrent.getZ(), dt));
			return result;
		};
		static float roundf(float x){
			   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
		};
    };
};