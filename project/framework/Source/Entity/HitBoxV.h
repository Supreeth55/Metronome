#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Asset;

class HitBoxV : public Entity
{
public:
	HitBoxV();
	virtual ~HitBoxV();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	int mFrameCounter;

	Body* pBd;
	Asset* pAs;


};