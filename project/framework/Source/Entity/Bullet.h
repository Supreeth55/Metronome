#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class Bullet : public Entity
{
public:
	Bullet();
	virtual ~Bullet();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	Body* pBd;
	Animation* pAn;
	Transform* pTr;
	int framecounter = 0;

};
