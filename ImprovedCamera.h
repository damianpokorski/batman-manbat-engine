#include "Engine.h"
using namespace std;
using namespace Manbat;
namespace Manbat {
	class ImprovedCamera
	{
	public:
		// Camera related stuff
		Camera* camera;
		Mesh* cameraCollision;
		Vector3 cameraOffset;
		Vector3 cameraOffsetDefault;
		// Character related data
		Vector3 currentVelocity;
		Vector3 goalVelocity;

		EAngle characterToCamera;
		Vector3 characterMovement;

		Vector3 vecForward;
		Vector3 vecRight;

		int characterSpeed;
		bool flying;
		// External velocity
		Vector3 externalVelocity;
		float VelocityDragPerSecond;
		ImprovedCamera() {
		};

		int enemyCollision;
		float invincibility;

		bool crouching;

		// triggers that let the owning gamestate perform wizards / react
		bool collectableHitEnergy;
		bool collectableHitHealth;
		bool collectableHitInsulin;
		bool hitByEnemy;
		bool jumped;
		bool canJump;
		bool hitFlag;
		// Sounds
		cSound* jumpSound;
		cSound* hitSound;
		cSound* PickupSound;
		void Init() {
			enemyCollision = 0;
			invincibility = 0;
			//  Settting up variables
			characterSpeed = 50;
			flying = false;
			cameraOffset = Vector3(0, 25, 0);
			cameraOffsetDefault = Vector3(0, 25, 0);
			externalVelocity = Vector3(0, 0, 0);
			VelocityDragPerSecond = 3;
			// Setting up the camera
			camera = new Camera();
			characterToCamera.p = 0.0f;
			characterToCamera.y = 0.0f;
			characterToCamera.r = 0.0f;

			camera->setPosition(400, 10, -140);
			camera->setTarget(1, 0, 0);
			camera->Update();

			// Player collision
			cameraCollision = new Mesh();
			cameraCollision->createCube(10, 30, 10);
			cameraCollision->setPosition(camera->getPosition());
			cameraCollision->setEntityType(ENTITY_PLAYER_MESH);
			cameraCollision->setAlive(true);
			cameraCollision->setCollidable(true);
			// trigger reset
			collectableHitEnergy = false;
			collectableHitHealth = false;
			collectableHitInsulin = false;
			hitByEnemy = false;
			jumped = false;
			canJump = false;
			hitFlag = false;
			crouching = false;

			jumpSound = new cSound("Sound/Jump.wav");
			hitSound = new cSound("Sound/Hit.wav");
			PickupSound = new cSound("Sound/Pickup.wav");

		};

		void PlayerToEnemyCollision(Mesh* enemy) {
			// If player is within invincibility frames do not do anything.
			if (invincibility <= 0) {
				externalVelocity = Vector3(0, 500, 0);
				cameraCollision->grounded = false;
				enemyCollision++;
				invincibility = 0.2f;
				hitByEnemy = true;
				hitSound->Play();
			}
		};
		void PlayerToCollectableCollision(ImprovedCollectable* collectable) {
			if (collectable->Disabled)
				return;
			switch (collectable->collectableType) {
				case ImprovedCollectable::CollectableType::Energy:
					collectableHitEnergy = true;
				break;
				case ImprovedCollectable::CollectableType::Health:
					collectableHitHealth = true;
				break;
				case ImprovedCollectable::CollectableType::Insulin:
					collectableHitInsulin = true;
				break;
			}
			collectable->Disabled = true;
			PickupSound->Play();
		};
		float Approach(float flGoal, float flCurrent, float dt) {
			dt = dt * 5;
			float flDifference = flGoal - flCurrent;

			if (flDifference > dt)
				return flCurrent + dt;
			if (flDifference < -dt)
				return flCurrent - dt;

			return flGoal;
		};

		void HandleGravity(float deltaTime) {
			// Arbitary gravity - 9.8 is just too slow. / Considering 2m block seems about 50 units - realistic gravity should be ... 50*-9.8
			if (!cameraCollision->grounded || !cameraCollision->isCollided()) {
				externalVelocity.setY(externalVelocity.getY() + (-490.0f * deltaTime));
			}
			if (flying) {
				externalVelocity = Vector3(0, 0, 0);
			}
		};
		void Update(float deltaTime) {

			invincibility -= deltaTime;
			
			// Inheriting camera position from collider
			camera->setPosition(cameraCollision->getPosition());

			// The collision fix
			if (cameraCollision->PositionReseted) {
				camera->setPosition(cameraCollision->PreviousPosition);
				cameraCollision->PositionReseted = false;
			}

			// Smooth velocity
			characterMovement = Math::Approach(goalVelocity, characterMovement, deltaTime * 100);
			vecForward = characterToCamera.ToVector();
			if (!flying) {
				vecForward.y = 0;
			}

			vecForward = Math::Normal(vecForward);
			Vector3 vecUp(0, 1, 0);
			vecRight = Math::crossProduct(vecUp, vecForward);
			currentVelocity = vecForward * -characterMovement.z + vecRight *-characterMovement.x;

			// Gravity MAGIC 
			HandleGravity(deltaTime);

			// Smooth out external velocity
			externalVelocity = externalVelocity - (externalVelocity * (VelocityDragPerSecond*deltaTime));
			externalVelocity.setX(externalVelocity.getX() - (externalVelocity.getX() * (VelocityDragPerSecond*deltaTime)));
			externalVelocity.setZ(externalVelocity.getZ() - (externalVelocity.getZ() * (VelocityDragPerSecond*deltaTime)));

			camera->setPosition(
				camera->getPosition().getX() + (currentVelocity.getX() * deltaTime) + (externalVelocity.getX() * deltaTime) + cameraOffset.getX(),
				camera->getPosition().getY() + (currentVelocity.getY() * deltaTime) + (externalVelocity.getY() * deltaTime) + cameraOffset.getY(),
				camera->getPosition().getZ() + (currentVelocity.getZ() * deltaTime) + (externalVelocity.getZ() * deltaTime) + cameraOffset.getZ()
				);
			Vector3 camTarg = characterToCamera.ToVector();
			//camera->setPosition(playerCollision->getPosition());
			camera->setTarget(camera->getPosition() + camTarg);
			
			// Now playercollision is inheriting position from player collision... ?
			// The logistics of this are killing me.
			cameraCollision->setPosition(camera->getPosition()-cameraOffset);
			cameraCollision->Update(deltaTime);

			// Updating the camera
			camera->Update();
		};

		void InputEvent(IEvent* e) {
			switch (e->GetID()) {
			case EventType::EVENT_KEYPRESS:
				switch (((KeyPressEvent*)e)->keycode) {
				case DIK_W:
					goalVelocity.setZ(-characterSpeed);
					break;
				case DIK_S:
					goalVelocity.setZ(characterSpeed);
					break;
				case DIK_A:
					goalVelocity.setX(characterSpeed);
					break;
				case DIK_D:
					goalVelocity.setX(-characterSpeed);
					break;
				case DIK_LCONTROL:
					if (!crouching) {
						crouching = true;
						Debug << "[Camera] Crouching" << std::endl;
					}
						
					break;
				}
				break;


			case EventType::EVENT_KEYRELEASE:

				switch (((KeyReleaseEvent*)e)->keycode) {
				case DIK_W:
					goalVelocity.setZ(0);
					break;
				case DIK_S:
					goalVelocity.setZ(0);
					break;
				case DIK_A:
					goalVelocity.setX(0);
					break;
				case DIK_D:
					goalVelocity.setX(0);
					break;
				case DIK_SPACE:
					if (cameraCollision->grounded && canJump) {
						externalVelocity.setY(250);
						cameraCollision->grounded = false;
						jumped = true;
						jumpSound->Play();
					}
					break;
				case DIK_LCONTROL:
					crouching = false;
					break;
				case DIK_F1:
					flying = !flying;
					if (flying) {
						characterSpeed *= 5;
					}
					else {
						characterSpeed /= 5;
					}
					break;
				}
				break;
			case EventType::EVENT_MOUSEMOTION:
			{
				MouseMotionEvent* mme = (MouseMotionEvent*)e;
				double movex = mme->deltax;
				double movey = mme->deltay;

				characterToCamera.y -= movex * 0.0005;
				characterToCamera.p -= movey * 0.0005;

				if (characterToCamera.p >= 1.565)
					characterToCamera.p = 1.565;
				if (characterToCamera.p <= -1.565)
					characterToCamera.p = -1.565;

				characterToCamera.Normalize();
			}
			break;

			case EventType::EVENT_XBUTTON: {
				XButtonEvent* xbe = (XButtonEvent*)e;
				if (xbe->padPointer->IsConnected()) {
					float LeftX = (float)xbe->PadState.Gamepad.sThumbLX / 32768.0f;
					float LeftY = (float)xbe->PadState.Gamepad.sThumbLY / 32768.0f;
					float RightX = (float)xbe->PadState.Gamepad.sThumbRX / 32768.0f;
					float RightY = (float)xbe->PadState.Gamepad.sThumbRY / 32768.0f;
					// Deadzone implementation
					if (abs(LeftX) < 0.2f) {
						LeftX = 0.0f;
					}
					if (abs(LeftY) < 0.2f) {
						LeftY = 0.0f;
					}
					if (abs(RightX) < 0.2f) {
						RightX = 0.0f;
					}
					if (abs(RightY) < 0.2f) {
						RightY = 0.0f;
					}
					//
					goalVelocity.setX(LeftX*-characterSpeed);
					goalVelocity.setZ(LeftY*-characterSpeed);
					characterToCamera.y -= RightX  * 0.01;
					characterToCamera.p += RightY  * 0.01;
					// button handling
					if ((xbe->PadState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) {
						if (cameraCollision->grounded && canJump) {
							externalVelocity.setY(250);
							cameraCollision->grounded = false;
							jumped = true;
							jumpSound->Play();
						}
					}
					if ((xbe->PadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) {
						crouching = true;
					}
					else {
						crouching = false;
					}

				}
			}
			}
		};
	};
};
