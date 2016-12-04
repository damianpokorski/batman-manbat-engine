#pragma once

#include "Engine.h"

namespace Manbat{
	class cSound;
	class cSoundLayer{
		private:
			
		public:
			LPDIRECTSOUND8 m_pDSound;
			LPDIRECTSOUNDBUFFER8 m_pPrimary;
			static cSoundLayer* m_pGlobalSLayer;
			cSoundLayer(HWND hWnd);
			virtual ~cSoundLayer();
			static cSoundLayer* GetSound(){
				return m_pGlobalSLayer;
			}
			LPDIRECTSOUND8 GetDSound(){
				return m_pDSound;
			}
			static void Create(HWND hWnd){
				new cSoundLayer(hWnd);
			}
	};

	inline cSoundLayer* Sound(){
		return cSoundLayer::GetSound();
	}
}