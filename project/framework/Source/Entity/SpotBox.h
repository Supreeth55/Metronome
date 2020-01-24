#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Body;
class Transform;
class Animation;

class SpotBox : public Entity
{
public:
	SpotBox();
	virtual ~SpotBox();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);
	void CreateSpotLight();
public:
	unsigned int key;//entity position in the pool
	int colliding = 0;
	bool lit = false;
	Body* pBd;
	Transform* pTr;
	bool mGoingUp = true;
	float mTimer;
	float mTimerLimit;
};