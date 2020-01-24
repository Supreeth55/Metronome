#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Animation;

class HealthBar : public Entity
{
public:
	HealthBar();
	virtual ~HealthBar();

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
