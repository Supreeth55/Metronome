#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class TimeBar : public Entity
{
public:
	TimeBar();
	virtual ~TimeBar();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	void ReScale(float percent);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool
	float xscale;
	float prevH;

	Body* pBd;
	Transform* pTr;
};
