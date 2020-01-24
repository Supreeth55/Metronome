#include "../pch.h"
#include "Player.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Controller.h"
#include "../Components/Body.h"
#include "../Components/Animation.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../Entity/HealthBar.h"
#include "../Entity/PointLight.h"
#include "..//..//Manager/CollisionManager.h"
#include "../Entity/TimeBar.h"




Player::Player() : Entity(ENTITY_TYPE::PLAYER)
{
	health = 100.0f;
	if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
	{
		
	}
	else
	{
		time = 0.0f;
	}
	time = 10000.0f;
	mDashCounter = 0;
}

Player::~Player() {}

void Player::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(1, 1, 0);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}

	//Controller
	dataRoot = root["Controller"];

	if (!dataRoot.isNull())
	{
		Controller* pCr = new Controller();
		pCr->Serialize(dataRoot, position);
		this->pCr = pCr;
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Player";
	key = position;
	this->complete = true;
	CreateUI();
	CreatePointLight();
}

void Player::Update(float deltaTime)
{
	if (health >= 100)
	{
		health = 100;
	}
	mWeapon = 0.85f * (Equipped + 1);
	mRangedWeapon = 0.85f * (REquipped + 1);
	if (mDashCounter > 0)
	{
		mDashCounter--;
		if (pBd->mFacingUp == true && pBd->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 0.2f / sqrtf(2.0f);
		}
		else if (pBd->mFacingUp == true && pBd->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 0.2f / sqrtf(2.0f);
		}
		else  if (pBd->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y += 0.2f;
		}
		if (pBd->mFacingDown == true && pBd->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 0.2f / sqrtf(2.0f);
		}
		else if (pBd->mFacingDown == true && pBd->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 0.2f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 0.2f / sqrtf(2.0f);
		}
		else if (pBd->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y -= 0.2f;
		}
		if (pBd->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x += 0.2f;
		}
		else if (pBd->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x -= 0.2f;
		}
	}
	pCr->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
	if (!pCr)
	{
		std::cout << "not pcr" << std::endl;
	}
	if (pBd)
		pBd->Update(deltaTime);



	for (int k = 0; k < ObjectFactoryManager::GetInstance()->LLightPositions.size(); k++)
	{
		int light_pos = ObjectFactoryManager::GetInstance()->LLightPositions.at(k);
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid((*MemoryManager::GetInstance()->ObjectPool)[light_pos].pBd->mShape,
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos].pBd->mPos_x, (*MemoryManager::GetInstance()->ObjectPool)[light_pos].pBd->mPos_y,
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos].pBd->mPos_z, pBd->mShape,pBd->mPos_x, pBd->mPos_y,pBd->mPos_z))
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pTr->mPositionX;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pTr->mPositionY;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = pTr->mPositionZ + 5.0f;
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos + 1].pLd->pLt->SetDiffuse(2.0f, 0.0f, 0.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos + 1].pLd->pLt->SetSpecular(1.0f, 0.0f, 0.0f);

			
			spotcheck = true;
			break;
		}
		else
		{

			
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pTr->mPositionX;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pTr->mPositionY;
			(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = pTr->mPositionZ + 50.0f;
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos+ 1].pLd->pLt->SetDiffuse(2.0f, 2.0f, 2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[light_pos + 1].pLd->pLt->SetSpecular(1.0f, 1.0f, 1.0f);
			spotcheck = false;
		}
	}


	if (eh)
	{
		
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionX = pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionY = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mPositionZ = pTr->mPositionZ + 5.0f;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[lpos].pTr->mPositionX = pTr->mPositionX;
	(*MemoryManager::GetInstance()->ObjectPool)[lpos].pTr->mPositionY = pTr->mPositionY;
	(*MemoryManager::GetInstance()->ObjectPool)[lpos].pTr->mPositionZ = pTr->mPositionZ + 4.5f;
	if (health < 0)
	{
		health = 0;
	}
	time-=0.001f;
	if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pTr->mPositionX = pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pTr->mPositionY = pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pTr->mPositionZ = pTr->mPositionZ + 10.0f;
	}
	
	(*MemoryManager::GetInstance()->ObjectPool)[key + 1].pTr->mScaleX = 2.0f * (health / 100.0f);
	(*MemoryManager::GetInstance()->ObjectPool)[key + 2].pTr->mScaleX *=(time / 10000.0f);
}

void Player::CreateUI()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RPG || MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
	{
		eh = true;
		ObjectFactoryManager::GetInstance()->LoadObject("HealthBar", curKey);
		
		//if (MemoryManager::GetInstance()->mProtType == PROT_TYPE::RUNNER)
		{
			ObjectFactoryManager::GetInstance()->LoadObject("TimeBar", curKey + 1);
		}
	}
	else
	{
		ObjectFactoryManager::GetInstance()->LoadObject("Cat", curKey);
	}
	
}

void Player::CreatePointLight()
{
	int curKey = MemoryManager::GetInstance()->current_position + 1;
	ObjectFactoryManager::GetInstance()->LoadObject("PointLight", curKey);
	lpos = curKey;
}
