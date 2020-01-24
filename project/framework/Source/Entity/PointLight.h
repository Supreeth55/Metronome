#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Transform;
class LightDetail;
class PointLight : public Entity
{
public:
	PointLight();
	virtual ~PointLight();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	Transform* pTr;
	LightDetail* pLd;
	//Light* pLt;

};
