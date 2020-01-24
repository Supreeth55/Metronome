#include "../pch.h"
#include "FirePlace.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

FirePlace::FirePlace() : Entity(ENTITY_TYPE::FIREPLACE)
{
	key = -1;
	pBd = nullptr;
}

FirePlace::~FirePlace() {}

void FirePlace::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];
	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}


	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "FirePlace";
	key = position;
	this->complete = true;

}

void FirePlace::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);

}
