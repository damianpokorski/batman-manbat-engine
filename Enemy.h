#pragma once
#include "Engine.h"
namespace Manbat {
	class Enemy : public ImprovedMesh {
	public:
		enum States {
			Roam,
			Seek,
			Attack,
			Cooldown,
			Taunt,
			Flee,
			Dead
		};
		Enemy::States currentState;
		Billboard* stateIndicator;
		float time;
		bool PingPong;

		Vector3 direction;
		Vector3 GoalDirection;
		float speed;
		Camera* playerController;

		bool attackAttempt;
		bool attackSuccess;

		Enemy() {
			// setting up the values
			attackAttempt = false;
			attackSuccess = false;
			time = 0;
			speed = 40;
			currentState = States::Roam;
			playerController = NULL;
			PingPong = true;
		};

		
		void Update(float deltaTime) {
			// update current direction based on the goal direction
			direction = Math::Approach(GoalDirection, direction, deltaTime*2);
			// update the direction every second, based on the current state
			time += deltaTime;
			if (time > 1) {
				stateLogic();
				GoalDirection = stateDirection(currentState);
				time = 0;
				PingPong = !PingPong;
			}
			// Update position based on the direction
			this->setPosition((direction*deltaTime*speed)+this->getPosition());
			if (PingPong) {
				this->setPosition(this->getPosition().getX(), 1 + time * 5, this->getPosition().getZ());
			}
			else {
				this->setPosition(this->getPosition().getX(), 1 + (1 -time) * 5, this->getPosition().getZ());
			}
			//this->setPosition(this->getPosition().getX(), 1, this->getPosition().getZ());
			Mesh::Update(deltaTime);
		}

		Vector3 stateDirection(States state) {
			// Wander state
			// distance between 0 and 100
			Vector3 goal = Vector3(0, 0, 0);
			switch (state) {
			case Roam:
				// Random direction
				goal = Vector3(-5 + (rand() % 10), 0, -5 + (rand() % 10));
				goal = Math::Normal(goal);
				speed = 40;
				break;
			case Seek:
				goal = playerController->getPosition() - this->getPosition();
				// dont chase up/down
				goal.setY(0);
				goal = Math::Normal(goal);
				speed = 20;
				break;
			case Flee:
				// Flee is just an inverted version of seek
				goal = this->getPosition() - playerController->getPosition();
				// dont chase up/down
				goal.setY(0);
				goal = Math::Normal(GoalDirection);
				speed = 80;
				break;
			case Attack:
				goal = stateDirection(Seek);
				speed = 80;
				break;
			case Cooldown:
				goal = Vector3(0, 0, 0);
				speed = 0;
				break;
			case Taunt:
				goal = Vector3(0, 0, 0);
				speed = 0;
				break;
			case Dead:
				goal = Vector3(0, 0, 0);
				speed = 0;
				break;
			}
			return goal;
		};
		void stateLogic();
	};
};