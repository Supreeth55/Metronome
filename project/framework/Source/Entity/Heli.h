#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;

class Heli : public Entity
{
public:
	Heli();
	virtual ~Heli();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
};
