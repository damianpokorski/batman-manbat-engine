#pragma once
#include "Engine.h"

namespace Manbat{
	class XboxPad{
	private:
		XINPUT_STATE _controllerState;
		int _controllerNum;
	public:
		XboxPad(int playerNumber);
		XINPUT_STATE GetState();
		bool IsConnected();
		void Vibrate(int leftVal = 0, int rightVal = 0);
		int getPlayerNum(){ return _controllerNum; };
		std::map<WORD, bool> buttonDown;
		bool buttonReleased(WORD XINPUT_BUTTON);
	};
}