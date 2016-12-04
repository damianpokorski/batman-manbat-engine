#pragma once

#include "Engine.h"

namespace Manbat{


    class Camera
    {
    private:
        Matrix p_matrixProj;
        Matrix p_matrixView;
        Matrix p_matrixRot;
        D3DXVECTOR3 p_updir;



        double p_nearRange;
        double p_farRange;
        double p_fov;

    public:
		Vector3 p_position;
		Vector3 p_rotation;
		Vector3 p_target;

		double p_aspectRatio;
        Camera(void);
        ~Camera(void);

        void setPerspective(double fov, double aspectRatio, double nearRange, double farRange);
        Matrix getProjMatrix() { return p_matrixProj; }
        Matrix getViewMatrix() { return p_matrixView; }

        //camera position helpers
        Vector3 getPosition() { return p_position; }
        void setPosition(double x, double y, double z);
        void setPosition(Vector3 vector) { p_position = vector; }

        void setRotation(double x,double y,double z);
        void setRotation(Vector3 vector) { p_rotation = vector; }

        //camera lookat helpers
        Vector3 getTarget() { return p_target; }
        void setTarget(Vector3 value) { p_target = value; }
        void setTarget(double x, double y, double z) 
        { 
            p_target.x = (float)x; 
            p_target.y = (float)y; 
            p_target.z = (float)z; 
        }

        void Update();
        void Rotate(double x, double y, double z);
        void Look(double x, double y, double z);
        void Move(double x, double y, double z);


    };

};