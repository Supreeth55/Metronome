#include "../Source/pch.h"
#include "ObjectFactoryManager.h"

#include<math.h>

#include "../Manager/ResourceManager.h"

#include "../Source/Components/Component.h"

#include "../Source/Components/Asset.h"
#include "../Source/Components/Transform.h"
#include "../Source/Components/Controller.h"
#include "../Source/Components/Animation.h"
#include "../Source/Components/LightDetail.h"

#include "../Source/Entity/Player.h"
#include "../Source/Entity/Bullet.h"
#include "../Source/Entity/Platform.h"

#include "../Source/Entity/Tree.h"
#include "../Source/Entity/Cat.h"
#include "../Source/Entity/FirePlace.h"
#include "../Source/Entity/Heli.h"
#include "../Source/Entity/LightList.h"
#include "../Source/Entity/Collider.h"
#include "../Source/Entity/Block.h"
#include "../Source/Entity/Nanosuit.h"
#include "../Source/Entity/Fish.h"
#include "../Source/Entity/Enemy.h"
#include "../Source/Entity/HitBoxH.h"
#include "../Source/Entity/HitBoxV.h"
#include "../Source/Entity/HealthBar.h"
#include "../Source/Entity/TimeBar.h"
#include "../Source/Entity/Entity.h"
#include "../Source/Entity/ViewBox.h"
#include "../Source/Entity/SpotBox.h"
#include "../Source/Entity/WEnemy.h"
#include "../Source/Entity/Seeker.h"
#include "../Source/Entity/SeekerBox.h"
#include "../Source/Entity/SpotLight.h"
#include "../Source/Entity/DirectLight.h"
#include "../Source/Entity/PointLight.h"
#include "../Source/Entity/P1Goal.h"
#include "../Source/Entity/P1Collectible.h"
#include "../Source/Entity/P3Trap.h"
#include "../Source/Entity/P3Goal.h"

#include "../Manager/MemoryManager.h"
#include "ImGUIManager.h"

ObjectFactoryManager::ObjectFactoryManager() {}

ObjectFactoryManager::~ObjectFactoryManager() {}

ObjectFactoryManager* ObjectFactoryManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new ObjectFactoryManager();
	}
	return mInstance;
}

void ObjectFactoryManager::LoadLevel(std::string pFileName)
{

	unsigned int position = MemoryManager::GetInstance()->Allocate();
	Json::Value root = ResourceManager::GetInstance()->LoadJson(pFileName)[pFileName];

	//if valid JSON file
	if (!root.isNull())
	{
		for (unsigned int i = 0; i < root.size(); ++i)
		{
			//Get the gameobject file
			Json::Value objectRoot = root[i];
			int curKey = MemoryManager::GetInstance()->current_position;

			//Check the datafile
			Json::Value dataFile = objectRoot["DataFile"];
			if (!dataFile.isNull())
			{
				std::string fileroot = dataFile.asString();
				//if (fileroot == "Light")
				//{
				//	Json::Value lightroot = objectRoot["Light"];

				//	/*Json::Value values = lightroot["DirectLight"];
				//	dl = values.asInt();
				//	values = lightroot["PointLight"];
				//	pl = values.asInt();
				//	values = lightroot["SpotLight"];*/

				//}
				LoadObject(dataFile.asString(), curKey);
				ReSerialize(objectRoot, curKey);
			}
		}
	}
	for (unsigned int i = 0; i < MemoryManager::GetInstance()->current_position; i++)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::PLATFORM)
		{
			LWallPositions.emplace_back(i);
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SPOTBOX)
		{
			LLightPositions.emplace_back(i);
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SEEKERBOX)
		{
			LBoxPositions.emplace_back(i);
		}
	}
}

void ObjectFactoryManager::LoadObject(std::string pFileName, unsigned int key)
{

	unsigned int position = key;
	Json::Value root = ResourceManager::GetInstance()->LoadJson(pFileName);

	if (!root.isNull())
	{
		Json::Value objectRoot = root["Player"];

		if (!objectRoot.isNull())
		{
			Player* pl = new Player();
			pl->Serialize(objectRoot, position);
		}

		objectRoot = root["Bullet"];

		if (!objectRoot.isNull())
		{
			Bullet* bl = new Bullet();
			bl->Serialize(objectRoot, position);
		}

		objectRoot = root["Platform"];
		if (!objectRoot.isNull())
		{
			Platform* pl = new Platform();
			pl->Serialize(objectRoot, position);
		}

		objectRoot = root["Block"];
		if (!objectRoot.isNull())
		{
			Block* pl = new Block();
			pl->Serialize(objectRoot, position);
		}

		objectRoot = root["Collider"];
		if (!objectRoot.isNull())
		{
			Collider* pl = new Collider();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["SeekerBox"];
		if (!objectRoot.isNull())
		{
			SeekerBox* pl = new SeekerBox();
			pl->Serialize(objectRoot, position);
		}
		objectRoot = root["Cat"];
		if (!objectRoot.isNull())
		{
			Cat* cat = new Cat();
			cat->Serialize(objectRoot, position);
		}

		objectRoot = root["Tree"];
		if (!objectRoot.isNull())
		{
			Tree* tre = new Tree();
			tre->Serialize(objectRoot, position);
		}

		objectRoot = root["Heli"];
		if (!objectRoot.isNull())
		{
			Heli* hel = new Heli();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Nanosuit"];
		if (!objectRoot.isNull())
		{
			Nanosuit* hel = new Nanosuit();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Fish"];
		if (!objectRoot.isNull())
		{
			Fish* hel = new Fish();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["Seeker"];
		if (!objectRoot.isNull())
		{
			Seeker* hel = new Seeker();
			hel->Serialize(objectRoot, position);
		}
		objectRoot = root["FirePlace"];
		if (!objectRoot.isNull())
		{
			FirePlace* fp = new FirePlace();
			fp->Serialize(objectRoot, position);
		}

		objectRoot = root["Enemy"];
		if (!objectRoot.isNull())
		{
			Enemy* fp = new Enemy();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["WEnemy"];
		if (!objectRoot.isNull())
		{
			WEnemy* fp = new WEnemy();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["ViewBox"];
		if (!objectRoot.isNull())
		{
			ViewBox* fp = new ViewBox();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["SpotBox"];
		if (!objectRoot.isNull())
		{
			SpotBox* fp = new SpotBox();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["HitBoxH"];
		if (!objectRoot.isNull())
		{
			HitBoxH* fp = new HitBoxH();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["HitBoxV"];
		if (!objectRoot.isNull())
		{
			HitBoxV* fp = new HitBoxV();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["HealthBar"];
		if (!objectRoot.isNull())
		{
			HealthBar* fp = new HealthBar();
			fp->Serialize(objectRoot, position);
		}


		objectRoot = root["SpotLight"];
		if (!objectRoot.isNull())
		{
			SpotLight* fp = new SpotLight();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["DirectLight"];
		if (!objectRoot.isNull())
		{
			DirectLight* fp = new DirectLight();
			fp->Serialize(objectRoot, position);
		}

		objectRoot = root["PointLight"];
		if (!objectRoot.isNull())
		{
			PointLight* fp = new PointLight();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["TimeBar"];
		if (!objectRoot.isNull())
		{
			TimeBar* fp = new TimeBar();
			fp->Serialize(objectRoot, position);
		}

		/*objectRoot = root["LightList"];
		if (!objectRoot.isNull())
		{
			PointLight* fp = new PointLight();
			fp->Serialize(objectRoot, position);
		}*/

		objectRoot = root["P1Goal"];
		if (!objectRoot.isNull())
		{
			P1Goal* fp = new P1Goal();
			fp->Serialize(objectRoot, position);
		}

		objectRoot = root["P1Collectible"];
		if (!objectRoot.isNull())
		{
			P1Collectible* fp = new P1Collectible();
			fp->Serialize(objectRoot, position);
		}

		objectRoot = root["P3Trap"];
		if (!objectRoot.isNull())
		{
			P3Trap* fp = new P3Trap();
			fp->Serialize(objectRoot, position);
		}
		objectRoot = root["P3Goal"];
		if (!objectRoot.isNull())
		{
			P3Goal* fp = new P3Goal();
			fp->Serialize(objectRoot, position);
		}

		MemoryManager::GetInstance()->current_position++;
	}
}


void ObjectFactoryManager::ReSerialize(Json::Value objectRoot, unsigned int key)
{
	//Transform
	Json::Value transform = objectRoot["Transform"];
	if (!transform.isNull())
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->Serialize(transform, key);

		if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionY;
			(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionZ;
		}
		/*pBd->mPos_x = pTr->mPositionX;
		pBd->mPos_y = pTr->mPositionY;
		pBd->mPos_z = pTr->mPositionZ;*/
		//}
		//reserialize body to reset the body position and shape


	/*	if (pBd)
		{
			pBd->Serialize(objectRoot, key);
		}*/
	}
	//Body
	Json::Value body = objectRoot["Body"];
	if (!body.isNull())
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->Serialize(body, key);
		/*if (pBd)
		{
			pBd->Serialize(body, key);
		}*/
	}
	//Asset
	Json::Value asset = objectRoot["Asset"];
	if (!asset.isNull())
	{
		Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[key].pAs;

		(*MemoryManager::GetInstance()->ObjectPool)[key].pAs->Serialize(asset, key);

		Json::Value dataRoot = asset["Diffuse"];
		if (!dataRoot.isNull())
		{
			(*MemoryManager::GetInstance()->ObjectPool)[key].pAs->obj->SetDiffuse(dataRoot[0].asFloat(), dataRoot[1].asFloat(), dataRoot[2].asFloat());

		}


	}
	//Controller
	Json::Value contr = objectRoot["Controller"];
	if (!contr.isNull())
	{
		Controller* pCr = (*MemoryManager::GetInstance()->ObjectPool)[key].pCr;
		if (pCr)
		{
			pCr->Serialize(contr, key);
		}
	}
	//Asset
	//Json::Value asset = objectRoot["Asset"];
	//if (!asset.isNull())
	//{
	//	Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[key].pAs;
	//	if (pAs)
	//	{
	//		pAs->Serialize(asset, key);
	//	}
	//}
	//Animation
	Json::Value anim = objectRoot["Animation"];
	if (!anim.isNull())
	{
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[key].pAn;
		if (pAn)
		{
			pAn->Serialize(anim, key);
		}
	}
	Json::Value ligdetail = objectRoot["LightDetail"];
	if (!ligdetail.isNull())
	{
		LightDetail* pLd = (*MemoryManager::GetInstance()->ObjectPool)[key].pLd;
		if (pLd)
		{
			pLd->Serialize(ligdetail, key);
		}
	}
}
void ObjectFactoryManager::FireBullet(Body* ptr, int curKey)
{
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionX = ptr->mPos_x;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY = ptr->mPos_y;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ = ptr->mPos_z;
	if ((*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x = ptr->mPos_x;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y = ptr->mPos_y;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_z = ptr->mPos_z;

		if (ptr->mFacingUp == true && ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = 5.0f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = 5.0f / sqrtf(2.0f);
		}
		else if (ptr->mFacingUp == true && ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = 5.0f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = -5.0f / sqrtf(2.0f);
		}
		else  if (ptr->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = 5.0f;
		}
		if (ptr->mFacingDown == true && ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = -5.0f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = -5.0f / sqrtf(2.0f);
		}
		else if (ptr->mFacingDown == true && ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = -5.0f / sqrtf(2.0f);
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = 5.0f / sqrtf(2.0f);
		}
		else if (ptr->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_y = -5.0f;
		}
		if (ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = 5.0f;
		}
		else if (ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_x = -5.0f;
		}
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mVelo_z = 0.0f;
		(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mAcc_z = 0.0f;
	}
}
void ObjectFactoryManager::DropBox(const char* pFileName)
{
	srand((unsigned int)time(0));
	int curKey = MemoryManager::GetInstance()->current_position;
	LoadObject(pFileName, curKey);
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionX = -17.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY = 0.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ = 100.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_x = -17.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_y = 0.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->mPos_z = 100.0f;
	(*MemoryManager::GetInstance()->ObjectPool)[curKey].pAs->obj->SetDiffuse(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f);
}

void ObjectFactoryManager::CreateAmmoPack()
{
	start = MemoryManager::GetInstance()->current_position;
	//create ammo from key to key+9, so 10 fireballs.
	for (int i = 0; i < 10; i++)
	{
		int curKey = MemoryManager::GetInstance()->current_position;
		LoadObject("Bullet", curKey);
		//(*MemoryManager::GetInstance()->ObjectPool)[curKey].pBd->active = false;
	}
}


void ObjectFactoryManager::DeleteObject()
{
	if (MemoryManager::GetInstance()->current_position > 1)
		MemoryManager::GetInstance()->Free();
}


void ObjectFactoryManager::Update(float rdeltaTime)
{
}

void ObjectFactoryManager::Destroy()
{
	delete mInstance;
}
void ObjectFactoryManager::SaveLevel()
{
	Json::StreamWriterBuilder builder;
	Json::StreamWriter* writer = builder.newStreamWriter();


	unsigned int numObj = MemoryManager::GetInstance()->current_position;
	std::unordered_map<unsigned, MemoryManager::ObjectNode> mObjPool = (*MemoryManager::GetInstance()->ObjectPool);
	Json::Value root(Json::objectValue);
	std::string filename = ImGUIManager::GetInstance()->filename;
	root[filename] = Json::arrayValue;


	for (unsigned int i = 0; i < numObj; ++i)
	{
		std::string name = mObjPool[i].pEn->mEntityName;

		Json::Value myObj(Json::objectValue);
		myObj["DataFile"] = name;
		if (mObjPool[i].pTr)
		{
			mObjPool[i].pTr->DeSerialize(myObj);
		}
		if (mObjPool[i].pAs)
		{
			mObjPool[i].pAs->DeSerialize(myObj);
		}
		if (mObjPool[i].pAn)
		{
			mObjPool[i].pAn->DeSerialize(myObj);
		}
		if (mObjPool[i].pCr)
		{
			mObjPool[i].pCr->DeSerialize(myObj);
		}
		if (mObjPool[i].pBd)
		{
			mObjPool[i].pBd->DeSerialize(myObj);
		}

		root[filename].append(myObj);
	}
	std::ofstream outputFile("Resource/Levels/" + filename + ".json");
	writer->write(root, &outputFile);
	outputFile.close();
}