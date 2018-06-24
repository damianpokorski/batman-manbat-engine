#pragma once
#include "Engine.h"

namespace Manbat {

	enum enemyLifeState {
		ENEMY_ALIVE,
		ENEMY_INJURED,
		ENEMY_SHOOT,
		ENEMY_DYING
	};

	enum enemyStateType {
		ENEMY_CASH_SEEK = 10,
		ENEMY_CASH_PURSUR = 20,
		ENEMY_CASH_HOLD = 30
	};

	class ScarletEnemy{
	private:
		BoneMesh* visibleModel;
		Mesh* enemyCollision;
		int enemyState;
		AStarNode* StartNode;
		AStarNode* GoalNode;
		AStarNode* CampNode;
		AStarNode* NextNode;
		Timer StarRecalcTime;
		Sprite* mapIcon;
		Vector3 forwardVector;
		float currentVelocity;
		std::vector<AStarNode*> PathList;
		bool gotPath;
		bool pathFinder;

		int health;
		int enemyLifeState;
		Timer damageTimer;
		Vector3 respawnPoint;
		Vector3 PlayerPos;
		Vector3 PlayerDir;
		bool JustShot;
		Timer ShootTimer;
		int weaponFired;


	public:
		ScarletEnemy();										//Constructer
		void Init();										//Initialise
		~ScarletEnemy();					//Destructor

		void Update(float deltaTime);				//Update
		void Render();
		void RenderPath();
		void CalculatePath();
		void CalculateNextNode();

		BoneMesh* getVisibleMesh()
		{
			return visibleModel;
		}
		Mesh* getInvisibleMesh()
		{
			return enemyCollision;
		}

		int getState()
		{
			return enemyState;
		}

		void setState(int value)
		{
			enemyState = value;
		}

		Sprite* getMap()
		{
			return mapIcon;
		}

		void setStartNode(AStarNode* node)
		{
			StartNode = node;
		}
		AStarNode* getStartNode()
		{
			return StartNode;
		}
		void setGoalNode(AStarNode* node)
		{
			GoalNode = node;
		}
		AStarNode* getGoalNode()
		{
			return GoalNode;
		}
		void setCampNode(AStarNode* node)
		{
			CampNode = node;
		}
		AStarNode* getCampNode()
		{
			return CampNode;
		}
		void setNextNode(AStarNode* node)
		{
			NextNode = node;
		}
		AStarNode* getNextNode()
		{
			return NextNode;
		}

		void setPathList(std::vector<AStarNode*> path)
		{
			PathList = path;
		}
		std::vector<AStarNode*> getPathList()
		{
			return PathList;
		}

		bool getPathFlag()
		{
			return gotPath;
		}
		void setPathFlag(bool value)
		{
			gotPath = value;
		}

		bool getPathFinder()
		{
			return pathFinder;
		}
		void setPathFinder(bool value)
		{
			pathFinder = value;
		}

		int getLifeState()
		{
			return enemyLifeState;
		}
		void setLifeState(int val)
		{
			enemyLifeState = val;
		}


		int getHealth()
		{
			return health;
		}
		void setHealth(int val)
		{
			health = val;
		}


		Vector3 getRespawnPoint()
		{
			return respawnPoint;
		}
		void RespawnPoint(Vector3 val)
		{
			respawnPoint = val;
		}


		bool getJustShot()
		{
			return JustShot;
		}
		void setJustShot(bool val)
		{
			JustShot = val;
		}


		Vector3 getPlayerPos()
		{
			return PlayerPos;
		}
		void setPlayerPos(Vector3 val)
		{
			PlayerPos = val;
		}


		Vector3 getPlayerDir()
		{
			return PlayerDir;
		}
		void setPlayerDir(Vector3 val)
		{
			PlayerDir = val;
		}

		void setWeaponFired(int value) {
			weaponFired = value;
		}

		void TakeDamage(int value);
	};
};
