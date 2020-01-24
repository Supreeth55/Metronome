#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class Collider : public Entity
{
public:
	Collider();
	virtual ~Collider();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Animation* pAn;


};
