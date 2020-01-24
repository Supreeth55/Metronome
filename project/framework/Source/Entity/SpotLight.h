#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"
class Transform;
class LightDetail;
class SpotLight : public Entity
{
public:
	SpotLight();
	virtual ~SpotLight();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	//void HandleEvent(Event *pEvent);

public:
	unsigned int key;//entity position in the pool

	Transform* pTr;
	LightDetail* pLd;
	//Light* pLt;

};
