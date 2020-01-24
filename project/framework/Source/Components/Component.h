/**
 * @file Component.h
 * @authors  Pratyush Gawai
 * @date 9/21/19
 * @brief Abstract component class
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef COMPONENT_H
#define COMPONENT_H


#include "../pch.h"
#include "../../Dependencies/jsoncpp/json/json.h"
#include "..\Events\Event.h"
#include "../Entity/Entity.h"

enum COMPONENT_TYPE
{
	TRANSFORM,
	ASSET,
	BODY, 
	CONTROLLER,
	ANIMATION,
	LIGHTDETAIL
};

//class Entity;
//class GameObject;
class Component
{
public:
	
	Component(COMPONENT_TYPE ctype);
	
	virtual ~Component();

	unsigned int GetType() { return mComponentType; }
	virtual void Update(float deltaTime) = 0;
	//virtual void Serialize(Json::Value root, unsigned int key) = 0;
	//virtual void Serialize(JSONObject root) {}
	//virtual void DeSerialize(Json::Value& root) = 0;
	

	virtual void HandleEvent(Event *EventToHandle) = 0;

public:
	Entity* mpEnowner;
	unsigned int CompPosition;

private:
	//Owner entity pointer
	//This will be needed to reference other component for the entity instance.
	//Entity *ownerEntity;
	
	COMPONENT_TYPE mComponentType;
};

#endif // !COMPONENT