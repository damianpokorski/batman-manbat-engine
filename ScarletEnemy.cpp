#include "Engine.h"

using namespace Manbat;

ScarletEnemy::ScarletEnemy() {

	visibleModel = new BoneMesh();
	visibleModel->Load("bone_all.x");

	enemyCollision = new Mesh();
	enemyCollision->createCube(2, 3, 2);

	enemyState = ENEMY_CASH_SEEK;

	mapIcon = new Sprite();
	mapIcon->Load("enemymap.png");

	forwardVector = Vector3(0, 0, -1);
	currentVelocity = 5;

	gotPath = false;

	pathFinder = false;

	health = 100;
	respawnPoint = Vector3(0, 0, 0);
	PlayerPos = Vector3(0, 0, 0);
	JustShot = false;
	weaponFired = 0;


}

ScarletEnemy::~ScarletEnemy() {

	if (visibleModel)delete visibleModel;
	if (enemyCollision) delete enemyCollision;
	if (mapIcon) delete mapIcon;
}

void ScarletEnemy::Update(float deltaTime)
{

	if (enemyCollision->isCollided() &&
		enemyCollision->getCollideBuddy()->getEntityType() == ENTITY_RAY)
	{

		float dmg = 10;

	/*	if (weaponFired == WEAPON_SHOTGUN)
		{
			dmg = 25;
		}
		else
		{
			dmg = 10;
		}*/

		TakeDamage((int)dmg);

	}

	if (enemyCollision->isCollided() &&
		enemyCollision->getCollideBuddy()->getEntityType() == ENTITY_TNT)
	{

		float dmg = 40;
		TakeDamage((int)dmg);
	}

	if (enemyLifeState == ENEMY_SHOOT) {

		Vector3 currentPos = enemyCollision->getPosition();

		forwardVector = Math::Normal(PlayerPos - currentPos);

		Vector3 vecX(1, 0, 0);

		double scalar = Math::dotProduct(forwardVector, vecX);

		double rotationValue = acos(scalar);

		if (forwardVector.z > 0)
		{

			double toAdd = PI - rotationValue;
			rotationValue = PI + toAdd;
		}

		rotationValue = D3DXToDegree(rotationValue);

		visibleModel->setRotation(rotationValue - 90, 0, 0);

		visibleModel->setAnimationSet(3);
		visibleModel->Update(deltaTime);
		visibleModel->Transform();
		visibleModel->Animate(deltaTime, &visibleModel->getMatrix());

		if (JustShot)
		{
			JustShot = false;
		}

		if (ShootTimer.Stopwatch(400))
		{
			enemyLifeState = ENEMY_ALIVE;
		}
		else if (enemyLifeState == ENEMY_DYING)
		{
			visibleModel->setAnimationSet(4);
			visibleModel->Update(deltaTime);
			visibleModel->Transform();
			visibleModel->Animate(deltaTime, &visibleModel->getMatrix());

			if (damageTimer.Stopwatch(300))
			{

				enemyLifeState = ENEMY_ALIVE;
				health = 100;

				enemyCollision->setPosition(respawnPoint);
			}
		}

		else if (enemyLifeState == ENEMY_INJURED)
		{

			visibleModel->setAnimationSet(2);
			visibleModel->Update(deltaTime);
			visibleModel->Transform();
			visibleModel->Animate(deltaTime, &visibleModel->getMatrix());

			if (damageTimer.Stopwatch(200))
				enemyLifeState = ENEMY_ALIVE;

		}
		else
		{

			visibleModel->setAnimationSet(1);


			Vector3 currentPos = enemyCollision->getPosition();

			if (pathFinder) CalculateNextNode();

			Vector3 Goal(0, 0, 0);
			Goal.x = NextNode->x * 10;
			Goal.z = NextNode->y * -10;

			forwardVector = Math::Normal(Goal - currentPos);

			visibleModel->setPosition(visibleModel->getPosition() +
				(forwardVector*deltaTime*currentVelocity));

			Vector3 vecX(1, 0, 0);

			double scalar = Math::dotProduct(forwardVector, vecX);

			double rotationValue = acos(scalar);

			if (forwardVector.z > 0)
			{

				double toAdd = PI - rotationValue;
				rotationValue = PI + toAdd;
			}

			rotationValue = D3DXToDegree(rotationValue);

			visibleModel->setRotation(rotationValue - 90, 0, 0);

			visibleModel->Update(deltaTime);
			visibleModel->Transform();
			visibleModel->Animate(deltaTime, &visibleModel->getMatrix());

			enemyCollision->setPosition(visibleModel->getPosition());
			enemyCollision->Update(deltaTime);

			if (ShootTimer.Stopwatch(1000))
			{

				if (PlayerPos.z< enemyCollision->getPosition().z + 20 &&
					PlayerPos.z > enemyCollision->getPosition().z - 20)
				{
					if (PlayerPos.x < enemyCollision->getPosition().x + 20 &&
						PlayerPos.x > enemyCollision->getPosition().x - 20)
					{
						enemyLifeState = ENEMY_SHOOT;
						JustShot = true;
					}

				}
			}
		}

	}
}

void ScarletEnemy::CalculateNextNode()
{

	double zPos = -NextNode->y * 10;
	double xPos = NextNode->x * 10;

	if (visibleModel->getPosition().z > zPos - 1 &&
		visibleModel->getPosition().z < zPos + 1 &&
		visibleModel->getPosition().x > xPos - 1 &&
		visibleModel->getPosition().x < xPos + 1)
	{

		if (PathList.size() > 0) {

			PathList.pop_back();

			if (PathList.size() > 0)
			{
				NextNode = PathList.back();
			}
			else
			{

				NextNode = GoalNode;

				gotPath = false;

			}
		}
	}
}

void ScarletEnemy::RenderPath()
{
	for (size_t i = 0; i < PathList.size(); i++)
	{
		Sprite* mapPath = new Sprite();
		mapPath->Load("moltmap.png");
		mapPath->setPivot(Vector2(2, 2));

		mapPath->setPosition(PathList[i]->x * 4 + 780,
			PathList[i]->y * 4 + 500);
		mapPath->Render();
	}
}

void ScarletEnemy::TakeDamage(int value) {

	health -= value;

	enemyLifeState = ENEMY_INJURED;
	damageTimer.Reset();
}