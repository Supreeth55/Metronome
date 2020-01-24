#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;
class Asset;
class ViewBox : public Entity
{
public:
	ViewBox();
	virtual ~ViewBox();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool
	int colliding = 0;
	int ownerEnemy;
	Body* pBd;
	Animation* pAn;
	Asset* pAs;
};