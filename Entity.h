#pragma once

#include "Engine.h"

namespace Manbat
{

	/************************************************************************/
	/* Entity type used internally to identify type of entity                                                                     */
	/************************************************************************/
	enum EntityType{
		ENTITY_UNKNOWN = -1,
		ENTITY_MESH = 1,
		ENTITY_SPRITE,
		ENTITY_BONE_MESH,
		ENTITY_PLAYER_MESH,
		ENTITY_SCENERY_MESH,
		ENTITY_ENEMY_MESH,
		ENTITY_RAY,
		ENTITY_COLLECTABLE_MESH,
		ENTITY_BILLBOARD,
		ENTITY_MAINPLANE,
		ENTITY_IMPROVED_COLLECTABLE_MESH,
		ENTITY_FLAG
	};

	/************************************************************************/
	/* Render type used internally to determine how entity is rendered                                                                     */
	/************************************************************************/
	enum RenderType{
		RENDER_NONE = 0,
		RENDER_2D,
		RENDER_3D
	};

	class Entity
	{
	protected:

	public:
		int id;													//unique id
		std::string name;										//name
		bool alive;												//is alive?
		enum RenderType renderType;						//render type
		enum EntityType entityType;						//entity type
		float lifetimeStart;									//start of life
		float lifetimeCounter;								//how long lived
		float lifetimeLength;									//how long to live
		bool visible;											//is visible?
		bool collidable;										//is collidable?
		bool collided;											//has collided?
		Entity* collideBuddy;
		//constructor / destructor
		Entity(enum RenderType renderType);
		virtual ~Entity(){ };

		//virtual;  must be overridden
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		//get / set ID 
		void setID(int value){ id = value; }
		int getID() { return id; }

		//get / set rendertype
		void setRenderType(enum RenderType type) { renderType = type; }
		enum RenderType getRenderType(){ return renderType; }

		//get /set name
		std::string getName(){ return name; }
		void setName(std::string value){ name = value; }

		//get / set visible
		bool getVisible(){ return visible; }
		void setVisible(bool value){ visible = value; }

//get / set alive
		bool getAlive(){ return alive; }
		void setAlive(bool value){ alive = value; }

		//get / set lifetime
		float getLifetime(){ return lifetimeLength; }
		void setLifetime(float value){ lifetimeLength = value; }
		//get current counter
		float getLifetimeCounter(){ return lifetimeCounter; }
		//check if lifetime expired
		bool lifetimeExpired(){
			return (lifetimeStart + lifetimeCounter > lifetimeLength);	}

		/************************************************************************/
		/* Note: EntityType should not be used as an identifier; it determines the     type of
		entity in the engine, e.g. sprite, mesh, etc. For identification purposes, use
		ID or name.*/
		/************************************************************************/
		EntityType getEntityType(){ return entityType; }
		void setEntityType(enum EntityType value){ entityType = value; }

		//-----------------------------------------------------collision support
		//get / set collided flag
		bool isCollided() { return collided; }
		void setCollided(bool value){ collided = value; }

		//get / set collidable flag
		bool isCollidable() { return collidable; }
		void setCollidable(bool value){ collidable = value; }

		//get / set collide buddy
		void setCollideBuddy(Entity* entity){ collideBuddy = entity; }
		Entity*getCollideBuddy(){ return collideBuddy; }

		//time mangement for entities
		void addToLifetimeCounter(float value){ lifetimeCounter += value; }


	};
};

