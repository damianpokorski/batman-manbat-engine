#pragma once
#include "Engine.h"

using namespace std;
namespace Manbat {
	void Enemy::stateLogic() {
		switch (currentState) {
		case Roam:
			// if player is within "hearing range", start seeking
			if (Math::Distance(this->getPosition(), playerController->getPosition()) < 300) {
				currentState = Enemy::States::Seek;
			}
			break;
		case Seek:
			// lost track of player - go back to roaming
			if (Math::Distance(this->getPosition(), playerController->getPosition()) > 350) {
				currentState = Enemy::States::Roam;
			}
			// If player is crouching
			if (g_engine->playerController->crouching) {
			currentState = Enemy::States::Roam;
			}
			else if (Math::Distance(this->getPosition(), playerController->getPosition()) < 200) {
				currentState = Enemy::States::Attack;
				attackAttempt = true;
				attackSuccess = false;
			}
			
			break;
		case Attack:
			currentState = Enemy::States::Cooldown;
			attackAttempt = false;
			break;
		case Cooldown:
			if (attackSuccess) {
				currentState = Enemy::States::Taunt;
			}
			else {
				currentState = Enemy::States::Seek;
			}
			break;
		}
		// After logic is applied - set the indicator to display according image
		switch (currentState) {
		case Roam:
			stateIndicator->LoadTexture("HUD/StateIndicators/Roam.png");
			break;
		case Seek:
			stateIndicator->LoadTexture("HUD/StateIndicators/Seek.png");
			break;
		case Flee:
			stateIndicator->LoadTexture("HUD/StateIndicators/Flee.png");
			break;
		case Attack:
			stateIndicator->LoadTexture("HUD/StateIndicators/Attack.png");
			break;
		case Cooldown:
			stateIndicator->LoadTexture("HUD/StateIndicators/Cooldown.png");
			break;
		case Taunt:
			stateIndicator->LoadTexture("HUD/StateIndicators/Taunt.png");
			break;
		case Dead:
			stateIndicator->LoadTexture("HUD/StateIndicators/Dead.png");
			break;
		}
	}
}