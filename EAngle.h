#pragma once

#include "Engine.h"

namespace Manbat
{
	class EAngle{
	public:
		double p, y, r; //pitch / yaw/ roll

		EAngle(){
			p = y = r = 0;
		}

		EAngle(double pitch, double yaw, double roll) {
			p = pitch;
			y = yaw;
			r = roll;
		}

		Vector3 ToVector() const; 

		void Normalize();
	};

}