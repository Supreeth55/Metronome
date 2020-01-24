#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;

class Enemy : public Entity
{
public:
	Enemy();
	virtual ~Enemy();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);
	void FindPlayer();
	void CreateUI();
public:
	unsigned int key;//entity position in the pool
	int val = 0;
	float health;
	Body* pBd;
	Transform* pTr;
	ShapeAABB* pSearchArea;
	Body* pSearchBody;
	bool seek = true;
};
