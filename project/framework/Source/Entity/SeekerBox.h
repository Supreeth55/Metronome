#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;

class SeekerBox : public Entity
{
public:
	SeekerBox();
	virtual ~SeekerBox();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);
	void CreateUI();
public:
	unsigned int key;//entity position in the pool
	int val = 0;
	float health;
	Body* pBd;
	Transform* pTr;
	bool player_visible = false;
};
