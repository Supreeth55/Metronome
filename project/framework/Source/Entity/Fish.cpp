#include "../pch.h"
#include "Fish.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Fish::Fish() : Entity(ENTITY_TYPE::FISH)
{

}

Fish::~Fish() {}

void Fish::Serialize(Json::Value root, unsigned int position)
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
	}


	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Fish";
	key = position;
	this->complete = true;

}

void Fish::Update(float deltaTime)
{

}
