#pragma once
#include "Engine.h"

namespace Manbat{
	class CWaveSoundRead{
	private:
	
	public:
		CWaveSoundRead();
		~CWaveSoundRead();
		HRESULT Open(CHAR* strFilename);
		HRESULT Reset();
		HRESULT Read(UINT nSizeToRead, BYTE* pbData, UINT* pnSizeRead);
		HRESULT Close();
		// Was private.. but well.. w/e
		WAVEFORMATEX* m_pwfx;
		HMMIO m_hmmioIn;
		MMCKINFO m_ckIn;
		MMCKINFO m_ckInRiff;
	};
	class cSound{
		private:
			LPDIRECTSOUNDBUFFER8 m_pBuffer;
			int m_bufferSize;

			static std::map<CWaveSoundRead*,int> m_waveMap;
			void Init();
		public:
			CWaveSoundRead* m_pWaveSoundRead;
			cSound(char* filename);
			cSound(cSound& in);
			cSound& operator=( const cSound &in);
			virtual ~cSound();
			void Restore();
			void Fill();
			void Play(bool bLoop =false);
			bool IsPlaying();
			void Stop();
			void setPosition(int pos);
	};
};