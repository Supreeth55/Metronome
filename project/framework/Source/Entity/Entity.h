#pragma once

#include "../pch.h"
#include "../../Dependencies/jsoncpp/json/json.h"
//#include "../../Manager/MemoryManager.h"

enum class ENTITY_TYPE
{
	PLAYER,
	BULLET,
	PLATFORM,
	BLOCK,
	CAT,
	TREE,
	HELI,
	FIREPLACE,
	COLLIDER,
	NANOSUIT,
	FISH,
	ENEMY,
	HITBOXH,
	HITBOXV,
	HEALTHBAR,
	WENEMY,
	VIEWBOX,
	SPOTBOX,
	SPOT_LIGHT,
	DIRECT_LIGHT,
	POINT_LIGHT,
	SEEKER,
	SEEKERBOX,
	P1COLLECTIBLE,
	P1GOAL,
	P3TRAP,
	TIMEBAR,
	P3GOAL
};

class Entity
{
public:

	Entity(ENTITY_TYPE TYPE);
	virtual ~Entity();
	virtual void Update(float deltaTime) {}
	//ENTITY_TYPE GetEntityType();
	//virtual void Serialize(Json::Value root, unsigned int position) {}
	ENTITY_TYPE GetEntityType()
	{
		return mEntityType;
	}
	//virtual void HandleEvent(Event* pEvent) {}
public:
	unsigned int EntityPosition;
	ENTITY_TYPE mEntityType ;
	std::string mEntityName;
	std::string mDataFile;
	bool complete;
private:
	

};
