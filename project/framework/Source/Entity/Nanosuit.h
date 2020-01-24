#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"



class Nanosuit : public Entity
{
public:
	Nanosuit();
	virtual ~Nanosuit();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	
};
