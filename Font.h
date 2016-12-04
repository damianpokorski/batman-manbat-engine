// Isnt this pragma here redundant? 
#pragma once
#include "Engine.h"

// He's not the hero Gotham deserves, but we'll have to make-do
namespace Manbat{
	class Font{
		private:
			LPD3DXFONT fontObj;
		public:
			// The font is born
			Font(std::string name, int size);
			// Raze it
			~Font();
			void Print(int x,int y, std::string text, D3DCOLOR color=0xFFFFFFFF);
			// I took the liberty of capitalizing the function names here
			// You gotta keep it consistent maaan!
			int GetTextWidth(std::string text);
			int GetTextHeight(std::string text);
	};
};