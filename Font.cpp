#include "Engine.h"
using namespace std;

namespace Manbat{
	Font::Font(std::string name, int size){
		// You're nothing font object! You're nothing without me!
		fontObj=NULL;
		D3DXFONT_DESC fontDesc = {
			size,					// Height
			0,						//	Width
			0,						// Weight
			0,						// MIPLevels
			false,					// Italic
			DEFAULT_CHARSET,		// 
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_PITCH,
			""
		};
		// Lets copy some font names
		strcpy_s(fontDesc.FaceName,name.c_str());
		// lets make some fonts
		D3DXCreateFontIndirect(g_engine->getDevice(), & fontDesc, &fontObj);
	};
	// Destructooo
	Font::~Font(){
		// Release all the opressed fonts.
		// Equal opportunities !
		fontObj->Release();
	}
	void Font::Print(int x,int y, std::string text, D3DCOLOR color){
		// Obligatory Shrek reference
		RECT rekt = {x, y, 0,0};
		// Calculate the (sh)rectangle then draw the text
		fontObj->DrawText(NULL, text.c_str(), text.length(), &rekt, DT_CALCRECT, color);
		fontObj->DrawText(g_engine->getSpriteObject(), text.c_str(), text.length(), &rekt, DT_LEFT, color);
	}
	int Font::GetTextWidth(std::string text){
		RECT rekt = {0, 0, 0, 0};
		fontObj->DrawText(NULL, text.c_str(), text.length(), &rekt, DT_CALCRECT, 0xFFFFFFFF);
		return rekt.right;
	}
	int Font::GetTextHeight(std::string text){
		RECT rekt = {0, 0, 0, 0};
		fontObj->DrawText(NULL, text.c_str(), text.length(), &rekt, DT_CALCRECT, 0xFFFFFFFF);
		return rekt.bottom;
	}
}