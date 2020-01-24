#include "../pch.h"
#include "Tree.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Tree::Tree() : Entity(ENTITY_TYPE::TREE)
{
}

Tree::~Tree() {}

void Tree::Serialize(Json::Value root, unsigned int position)
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

	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		pAs->obj->SetDiffuse(0.0f, 0.392f, 0.0f);
		
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Tree";
	key = position;
	this->complete = true;

}

void Tree::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);

}
