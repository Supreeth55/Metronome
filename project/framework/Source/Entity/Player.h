#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Controller;
class Animation;
class Body;
class Transform;
class Player : public Entity
{
public:
	enum Weapons
	{
		Wooden_Sword,
		Iron_Sword
	};
	enum Ranged_Weapons
	{
		Slingshot,
		Fireball
	};
	Player();
	virtual ~Player();

	void Serialize(Json::Value root, unsigned int position);
	void Update(float deltaTime);
	void CreateUI();
	void CreatePointLight();
	//void HandleEvent(Event *pEvent);
	bool eh = false;
	bool sound = false;
public:
	unsigned int key;//entity position in the pool
	unsigned int hPos;
	Controller* pCr;
	Animation* pAn;
	Body* pBd;
	Transform* pTr;
	int lpos;
	float health;
	int mDashCounter;
	float time;
public:

	float mDefense = 10;
	float mAttack = 10;

	float mArmor;
	float mWeapon;
	float mRangedWeapon;
	bool spotcheck = false;
	Weapons Equipped = Wooden_Sword;
	Ranged_Weapons REquipped = Fireball;

};
