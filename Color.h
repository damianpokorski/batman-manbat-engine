#pragma once

#include "Engine.h"

namespace Manbat{
	class Color{
		public: 
			float r, g, b, a;
			virtual ~Color();
			
			Color();
			Color(const Color& color);
			Color(int R, int G, int B, int A);
			Color(float R, float G, float B, float A);
			Color& Color::operator=(const Color& c);

			void Set(int R, int G, int B, int A);
			void Set(float R, float G, float B, float A);

			D3DCOLOR ToD3DCOLOR();
			D3DXVECTOR4 ToD3DXVECTOR4();
	};
};