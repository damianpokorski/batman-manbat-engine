#include "Engine.h"

namespace Manbat{
	XboxPad::XboxPad(int playerNumber){
		_controllerNum = playerNumber -1;
	}

	XINPUT_STATE XboxPad::GetState(){
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
		XInputGetState(_controllerNum, &_controllerState);

		return _controllerState;
	}

	bool XboxPad::IsConnected(){
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState(_controllerNum, &_controllerState);
		if(Result==ERROR_SUCCESS){
			return true;
		}else{
			return false;
		}
	}

	void XboxPad::Vibrate(int leftVal, int rightVal){
		XINPUT_VIBRATION Vibration;
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;
		XInputSetState(_controllerNum, &Vibration);
	}
	bool XboxPad::buttonReleased(WORD XINPUT_BUTTON) {
		// check if button is down
		if ((_controllerState.Gamepad.wButtons & XINPUT_BUTTON) != 0) {
			if (buttonDown.find(XINPUT_BUTTON) != buttonDown.end()) {
				buttonDown.find(XINPUT_BUTTON)->second = true;
			}
			else {
				buttonDown.insert(std::pair<WORD, bool>(XINPUT_BUTTON, true));
			}
			return false;
		}// Button is not down
		else {
			// check if button was down
			if (buttonDown.find(XINPUT_BUTTON) != buttonDown.end()) {
				if (buttonDown.find(XINPUT_BUTTON)->second) {
					buttonDown.find(XINPUT_BUTTON)->second = false;
					return true;
				}
			}
		}
		return false;
	}
}
