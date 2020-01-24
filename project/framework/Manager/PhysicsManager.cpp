/**
 * @file PhysicsManager.cpp
 * @authors Romil Tendulkar, Vineet Amarjeet Dogra
 * @date 09/29/2019
 * @brief This class is used to call the integration function of the body for the physics update,
			and to check collisions and assign appropriate response
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "PhysicsManager.h"
#include "..\Source\Components\Body.h"
#include "..\Source\Components\Asset.h"
#include "..//Manager/MemoryManager.h"
#include "..\\Source\GameObject.h"
#include "..\\Source\GameStates\GameState.h"
#include "..\Source\Util\ShapeX.h"
#include "AudioManager.h"
#include "CollisionManager.h"

#include "../Source/Entity/Enemy.h"
#include "../Source/Entity/Player.h"
#include "../Source/Entity/Viewbox.h"



PhysicsManager* PhysicsManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PhysicsManager();
	}
	return mInstance;
}

//PhysicsManager::PhysicsManager(GameState *mpOwner)
//{
//	if (mInstance == nullptr)
//	{
//		mInstance = new PhysicsManager();
//	}
//	return mInstance;
//}



PhysicsManager::PhysicsManager()
{
	pmOwner = nullptr;

}


PhysicsManager::PhysicsManager(GameState* pOwner)
{
	pmOwner = pOwner;
}

PhysicsManager::~PhysicsManager()
{

}


void PhysicsManager::Update(float rdeltaTime)
{
	CollisionManager::GetInstance()->Reset();
	unsigned int key = MemoryManager::GetInstance()->current_position;
	for (unsigned int i = 0; i < key; ++i)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->Update(rdeltaTime);
	}
	for (unsigned int i = 0; i < key; ++i)
	{

		Body* b1 = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd;
		if (b1 && b1->active)
		{
			//ShapeX* pShape1 = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd.mShape;
			for (unsigned int j = i + 1; j < key; ++j)
			{
				if ((*MemoryManager::GetInstance()->ObjectPool)[j].pBd)
				{
					Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[j].pBd;
					if (b2 && b2->active)
					{
						if (CollisionManager::GetInstance()->CheckBroadPhase2(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
						{
							if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(b1->mShape, b1->mPos_x, b1->mPos_y, b1->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
							{
								//std::cout << "\nCollide";
								//AudioManager::GetInstance()->loadAudio("Impact.wav");
								Contact* pNewContact = new Contact();
								pNewContact->mpBodies[0] = b1;
								pNewContact->mpBodies[1] = b2;
								CollisionManager::GetInstance()->mContacts.push_back(pNewContact);
							}
						}
					}
				}
			}
		}
	}

	unsigned int n = MemoryManager::GetInstance()->current_position;
	for (unsigned int i = 0; i < n; ++i)
	{
		if (i == 1024) continue;
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->body_State = FALLING;
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mCollidingSideRight = false;
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mCollidingSideLeft = false;
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mCollidingSideIn = false;
			(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mCollidingSideOut = false;
		}
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
		{
			if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mpEnowner->GetEntityType() == ENTITY_TYPE::VIEWBOX)
			{

				
					unsigned int key = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->key;
					ViewBox* viewbox = static_cast<ViewBox*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
					viewbox->colliding = 0;
				
			}
		}
	}

	for (auto& c : CollisionManager::GetInstance()->mContacts)
	{
		int proxy_check = 0;
		{
			ENTITY_TYPE b1 = c->mpBodies[0]->mpEnowner->GetEntityType();
			ENTITY_TYPE b2 = c->mpBodies[1]->mpEnowner->GetEntityType();
			{
				if (b2 == ENTITY_TYPE::PLATFORM && (b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::BULLET || b1 == ENTITY_TYPE::COLLIDER || b1 == ENTITY_TYPE::BLOCK || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::VIEWBOX|| b1 == ENTITY_TYPE::WENEMY))
				{
					float top, bottom, left, right, in, out;
					top = abs((c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight) - (c->mpBodies[0]->mPos_z - c->mpBodies[0]->mHeight));
					bottom = abs((c->mpBodies[1]->mPos_z - c->mpBodies[1]->mHeight) - (c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight));
					left = abs((c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth));
					right = abs((c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth));
					in = abs((c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth));
					out = abs((c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						c->mpBodies[0]->body_State = ON_SURFACE;
						c->mpBodies[0]->mPos_z = c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight + c->mpBodies[0]->mHeight;
						if (b1 == ENTITY_TYPE::PLAYER || b1 == ENTITY_TYPE::SEEKER || b1 == ENTITY_TYPE::ENEMY || b1 == ENTITY_TYPE::WENEMY)
						{
							c->mpBodies[0]->mVelo_x *= c->mpBodies[1]->mFrictionOnSurface;
							c->mpBodies[0]->mVelo_y *= c->mpBodies[1]->mFrictionOnSurface;
						}
						if (b1 == ENTITY_TYPE::BULLET)
						{
							c->mpBodies[0]->mVelo_z = (c->mpBodies[0]->mVelo_z * 0.50f);
						}
						if (b1 == ENTITY_TYPE::BLOCK)
						{
							c->mpBodies[0]->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						c->mpBodies[0]->mPos_z = c->mpBodies[1]->mPos_z - c->mpBodies[1]->mHeight - c->mpBodies[0]->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[0]->mCollidingSideRight = true;
						//c->mpBodies[0]->mRightMovement = false;
						c->mpBodies[0]->mPos_x = c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth - c->mpBodies[0]->mWidth - 0.0001f;
						c->mpBodies[0]->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[0]->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						c->mpBodies[0]->mCollidingSideIn = true;
						//c->mpBodies[0]->mDownMovement = false;
						c->mpBodies[0]->mPos_y = c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth + c->mpBodies[0]->mDepth + 0.0001f;
						c->mpBodies[0]->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[0]->mVelo_y = 0;
						//	c->mpBodies[0]->mAcc_y = -10;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						c->mpBodies[0]->mCollidingSideOut = true;
						//c->mpBodies[0]->mUpMovement = false;
						c->mpBodies[0]->mPos_y = c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth - c->mpBodies[0]->mDepth - 0.0001f;
						c->mpBodies[0]->mVelo_y *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[0]->mVelo_y = 0;
						//	c->mpBodies[0]->mAcc_y = -60;
						}

					}
					else//condition for left
					{
						c->mpBodies[0]->mCollidingSideLeft = true;
						//c->mpBodies[0]->mLeftMovement = false;
						c->mpBodies[0]->mPos_x = c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth + c->mpBodies[0]->mWidth + 0.0001f;
						c->mpBodies[0]->mVelo_x *= -1;
						if (b1 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[0]->mVelo_x = 0;
						}
					}
				}//
				else if (b1 == ENTITY_TYPE::PLATFORM && (b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::BULLET || b2 == ENTITY_TYPE::COLLIDER || b2 == ENTITY_TYPE::BLOCK || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::VIEWBOX|| b2 == ENTITY_TYPE::WENEMY))
				{
					float top, bottom, left, right, in, out;
					top = abs((c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight) - (c->mpBodies[1]->mPos_z - c->mpBodies[1]->mHeight));
					bottom = abs((c->mpBodies[0]->mPos_z - c->mpBodies[0]->mHeight) - (c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight));
					left = abs((c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth));
					right = abs((c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth));
					in = abs((c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth));
					out = abs((c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top < bottom && top < right && top < left && top < in && top < out)//condition for top
					{
						c->mpBodies[1]->body_State = ON_SURFACE;
						c->mpBodies[1]->mPos_z = c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight + c->mpBodies[1]->mHeight;
						if (b2 == ENTITY_TYPE::PLAYER || b2 == ENTITY_TYPE::SEEKER || b2 == ENTITY_TYPE::ENEMY || b2 == ENTITY_TYPE::WENEMY)
						{
							c->mpBodies[1]->mVelo_x *= c->mpBodies[0]->mFrictionOnSurface;
							c->mpBodies[1]->mVelo_y *= c->mpBodies[0]->mFrictionOnSurface;
						}
						if (b2 == ENTITY_TYPE::BULLET)
						{
							c->mpBodies[1]->mVelo_z = abs(c->mpBodies[1]->mVelo_z * 0.50f);
						}
						if (b2 == ENTITY_TYPE::BLOCK)
						{
							c->mpBodies[1]->mVelo_z = 0;
						}
					}
					else if (bottom < top && bottom < right && bottom < left && bottom < in && bottom < out)//condition for bottom
					{
						c->mpBodies[1]->mPos_z = c->mpBodies[0]->mPos_z - c->mpBodies[0]->mHeight - c->mpBodies[1]->mHeight;
					}
					else if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[1]->mCollidingSideRight = true;
						//c->mpBodies[1]->mRightMovement = false;
						c->mpBodies[1]->mPos_x = c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth - c->mpBodies[1]->mWidth - 0.0001f;
						c->mpBodies[1]->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[1]->mVelo_x = 0;
						}
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						c->mpBodies[1]->mCollidingSideIn = true;
						//c->mpBodies[1]->mDownMovement = false;
						c->mpBodies[1]->mPos_y = c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth + c->mpBodies[1]->mDepth + 0.0001f;
						c->mpBodies[1]->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
						//	c->mpBodies[1]->mAcc_y = -10;
							c->mpBodies[1]->mVelo_y = 0;
						}
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						c->mpBodies[1]->mCollidingSideOut = true;
						//c->mpBodies[1]->mUpMovement = false;
						c->mpBodies[1]->mPos_y = c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth - c->mpBodies[1]->mDepth - 0.0001f;
						c->mpBodies[1]->mVelo_y *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[1]->mVelo_y = 0;
							
						}
					}
					else//condition for left
					{
						c->mpBodies[1]->mCollidingSideLeft = true;
						//c->mpBodies[1]->mLeftMovement = false;
						c->mpBodies[1]->mPos_x = c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth + c->mpBodies[1]->mWidth + 0.0001f;
						c->mpBodies[1]->mVelo_x *= -1;
						if (b2 == ENTITY_TYPE::PLAYER)
						{
							c->mpBodies[1]->mVelo_x = 0;
						}
					}
				}//
				if (b1 == ENTITY_TYPE::BLOCK && b2 == ENTITY_TYPE::BLOCK)
				{
					float top, bottom;
					top = abs((c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight) - (c->mpBodies[0]->mPos_z - c->mpBodies[0]->mHeight));
					bottom = abs((c->mpBodies[1]->mPos_z - c->mpBodies[1]->mHeight) - (c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (top <= bottom)//condition for top
					{
						c->mpBodies[0]->body_State = ON_SURFACE;
						//c->mpBodies[1]->body_State = ON_SURFACE;
						c->mpBodies[0]->mPos_z = c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight + c->mpBodies[0]->mHeight;
						c->mpBodies[0]->mVelo_z = 0;
					}
					else if (bottom <= top)//condition for bottom
					{
						c->mpBodies[1]->mVelo_z = 0;
						//c->mpBodies[0]->body_State = ON_SURFACE;
						c->mpBodies[1]->body_State = ON_SURFACE;
						c->mpBodies[1]->mPos_z = c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight + c->mpBodies[1]->mHeight;
					}
				}//
				else if (b2 == ENTITY_TYPE::COLLIDER && b1 == ENTITY_TYPE::COLLIDER)
				{
					float top, bottom, left, right, in, out;
					top = abs((c->mpBodies[0]->mPos_z + c->mpBodies[0]->mHeight) - (c->mpBodies[1]->mPos_z - c->mpBodies[1]->mHeight));
					bottom = abs((c->mpBodies[0]->mPos_z - c->mpBodies[0]->mHeight) - (c->mpBodies[1]->mPos_z + c->mpBodies[1]->mHeight));
					left = abs((c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth));
					right = abs((c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth));
					in = abs((c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth));
					out = abs((c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth));
					//std::cout << "top: " << top << ", bottom: " << bottom << ", left: " << left << ", right: " << right << ", in: "<<in<<" out: "<< out<<  std::endl;
					if (right < top && right < bottom && right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[1]->mPos_x = c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth - c->mpBodies[1]->mWidth - 0.01f;
						//c->mpBodies[0]->mPos_x = c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth + c->mpBodies[1]->mWidth + 0.001;
						float init_momentum = c->mpBodies[1]->mVelo_x * c->mpBodies[1]->mMass + c->mpBodies[0]->mVelo_x * c->mpBodies[0]->mMass;
						float diff_v1v2;
						diff_v1v2 = (c->mpBodies[1]->mVelo_x - c->mpBodies[0]->mVelo_x);
						float m2diff = c->mpBodies[0]->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = c->mpBodies[1]->mVelo_x;
						c->mpBodies[1]->mVelo_x = Im_m2dif / (c->mpBodies[1]->mMass + c->mpBodies[0]->mMass);
						c->mpBodies[0]->mVelo_x = temp + c->mpBodies[1]->mVelo_x - c->mpBodies[0]->mVelo_x;
					}
					else if (in < right && in < left && in < top && in < bottom && in < out)
					{
						c->mpBodies[1]->mPos_y = c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth + c->mpBodies[1]->mDepth + 0.01f;
						float init_momentum = c->mpBodies[1]->mVelo_y * c->mpBodies[1]->mMass + c->mpBodies[0]->mVelo_y * c->mpBodies[0]->mMass;
						float diff_v1v2;
						diff_v1v2 = (c->mpBodies[1]->mVelo_y - c->mpBodies[0]->mVelo_y);
						float m2diff = c->mpBodies[0]->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = c->mpBodies[1]->mVelo_y;
						c->mpBodies[1]->mVelo_y = Im_m2dif / (c->mpBodies[1]->mMass + c->mpBodies[0]->mMass);
						c->mpBodies[0]->mVelo_y = temp + c->mpBodies[1]->mVelo_y - c->mpBodies[0]->mVelo_y;
					}
					else if (out < right && out < left && out < top && out < bottom && out < in)
					{
						c->mpBodies[1]->mPos_y = c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth - c->mpBodies[1]->mDepth - 0.01f;
						//	c->mpBodies[0]->mPos_y = c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth + c->mpBodies[0]->mDepth + 0.001;
						float init_momentum = c->mpBodies[1]->mVelo_y * c->mpBodies[1]->mMass + c->mpBodies[0]->mVelo_y * c->mpBodies[0]->mMass;
						float diff_v1v2;
						diff_v1v2 = (c->mpBodies[1]->mVelo_y - c->mpBodies[0]->mVelo_y);
						float m2diff = c->mpBodies[0]->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = c->mpBodies[1]->mVelo_y;
						c->mpBodies[1]->mVelo_y = Im_m2dif / (c->mpBodies[1]->mMass + c->mpBodies[0]->mMass);
						c->mpBodies[0]->mVelo_y = temp + c->mpBodies[1]->mVelo_y - c->mpBodies[0]->mVelo_y;
					}
					else//condition for left
					{
						c->mpBodies[1]->mPos_x = c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth + c->mpBodies[1]->mWidth + 0.01f;
						float init_momentum = c->mpBodies[1]->mVelo_x * c->mpBodies[1]->mMass + c->mpBodies[0]->mVelo_x * c->mpBodies[0]->mMass;
						float diff_v1v2;
						diff_v1v2 = (c->mpBodies[1]->mVelo_x - c->mpBodies[0]->mVelo_x);
						float m2diff = c->mpBodies[0]->mMass * diff_v1v2;
						float Im_m2dif = init_momentum - m2diff;
						float temp = c->mpBodies[1]->mVelo_x;
						c->mpBodies[1]->mVelo_x = Im_m2dif / (c->mpBodies[1]->mMass + c->mpBodies[0]->mMass);
						c->mpBodies[0]->mVelo_x = temp + c->mpBodies[1]->mVelo_x - c->mpBodies[0]->mVelo_x;
					}

				}//
				if (b1 == ENTITY_TYPE::PLAYER && b2 == ENTITY_TYPE::ENEMY)
				{

					float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth));
					right = abs((c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth));
					in = abs((c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth));
					out = abs((c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth));
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[0]->key);
					Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pTr;
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;
					Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
					player->health -= 10;
					enemy->seek = false;
					if (right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[0]->mVelo_x = 10;
						c->mpBodies[1]->mVelo_x = -20;
					}
					else if (in < right && in < left && in < out)
					{
						c->mpBodies[0]->mVelo_y = -10;
						c->mpBodies[1]->mVelo_y = 20;
					}
					else if (out < right && out < left && out < in)
					{
						c->mpBodies[0]->mVelo_y = 10;
						c->mpBodies[1]->mVelo_y = -20;
					}
					else//condition for left
					{
						c->mpBodies[0]->mVelo_x = -10;
						c->mpBodies[1]->mVelo_x = 20;
					}
				}
				else if (b1 == ENTITY_TYPE::ENEMY && b2 == ENTITY_TYPE::PLAYER)
				{
					float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth));
					right = abs((c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth));
					in = abs((c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth));
					out = abs((c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth));
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[0]->key);
					Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pTr;
					(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = false;
					Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
					Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
					player->health -= 10;
					enemy->seek = false;
					if (right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[1]->mVelo_x = 10;
						c->mpBodies[0]->mVelo_x = -20;
					}
					else if (in < right && in < left && in < out)
					{
						c->mpBodies[1]->mVelo_y = -10;
						c->mpBodies[0]->mVelo_y = 20;
					}
					else if (out < right && out < left && out < in)
					{
						c->mpBodies[1]->mVelo_y = 10;
						c->mpBodies[0]->mVelo_y = -20;
					}
					else//condition for left
					{
						c->mpBodies[1]->mVelo_x = -10;
						c->mpBodies[0]->mVelo_x = 20;
					}
				}
				if (b1 == ENTITY_TYPE::ENEMY && (b2 == ENTITY_TYPE::HITBOXV || b2 == ENTITY_TYPE::HITBOXH || b2 == ENTITY_TYPE::BULLET))
				{

					float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth));
					right = abs((c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth) - (c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth));
					in = abs((c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth));
					out = abs((c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth) - (c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth));
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[0]->key);
					Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pTr;


					c->mpBodies[1]->active = false;
					if (right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[0]->mPos_x += 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[0]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else if (in < right && in < left && in < out)
					{
						c->mpBodies[0]->mPos_y -= 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[0]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else if (out < right && out < left && out < in)
					{
						c->mpBodies[0]->mPos_y += 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[0]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else//condition for left
					{
						c->mpBodies[0]->mPos_x -= 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[1]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[0]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[1]->key);
					//CollisionManager::GetInstance()->mContacts.remove(c);
				}
				else if (b2 == ENTITY_TYPE::ENEMY && (b1 == ENTITY_TYPE::HITBOXV || b1 == ENTITY_TYPE::HITBOXH || b1 == ENTITY_TYPE::BULLET))
				{
				float top = 0.0f, bottom = 0.0f, left = 0.0f, right = 0.0f, in = 0.0f, out = 0.0f;
					left = abs((c->mpBodies[1]->mPos_x + c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x - c->mpBodies[0]->mWidth));
					right = abs((c->mpBodies[1]->mPos_x - c->mpBodies[1]->mWidth) - (c->mpBodies[0]->mPos_x + c->mpBodies[0]->mWidth));
					in = abs((c->mpBodies[1]->mPos_y + c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y - c->mpBodies[0]->mDepth));
					out = abs((c->mpBodies[1]->mPos_y - c->mpBodies[1]->mDepth) - (c->mpBodies[0]->mPos_y + c->mpBodies[0]->mDepth));
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[1]->key);
					Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pTr;

					c->mpBodies[0]->active = false;
					//MemoryManager::GetInstance()->FreeAt(c->mpBodies[0]->key);
					//CollisionManager::GetInstance()->mContacts.remove(c);
					if (right < left && right < in && right < out)//condition for right
					{
						c->mpBodies[1]->mPos_x += 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[1]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else if (in < right && in < left && in < out)
					{
						c->mpBodies[1]->mPos_y -= 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[1]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else if (out < right && out < left && out < in)
					{
						c->mpBodies[1]->mPos_y += 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[1]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
					else//condition for left
					{
						c->mpBodies[1]->mPos_x -= 0.5;
						Enemy* enemy = static_cast<Enemy*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
						Player* player = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[2].pEn);
						if (c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXH || c->mpBodies[0]->mpEnowner->GetEntityType() == ENTITY_TYPE::HITBOXV)
						{
							enemy->health -= player->mAttack * player->mWeapon;
						}
						else
						{
							enemy->health -= player->mAttack * player->mRangedWeapon;
						}
						if (enemy->health <= 0)
						{
							c->mpBodies[1]->active = false;
							pTr->mScaleX = 0;
							pTr->mScaleY = 0;
							pTr->mScaleZ = 0;
						}
					}
				}
				if (b2 == ENTITY_TYPE::PLAYER && b1 == ENTITY_TYPE::VIEWBOX)
				{
					Asset* asset =(*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pAs;
					ViewBox* viewbox = static_cast<ViewBox*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key].pEn);
					viewbox->colliding = 1;
					asset->obj->SetDiffuse(1, 0, 0);
					//(*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[0]->key+1].pLd->pLt->SetDiffuse(1, 0, 0);
					
				}
				else if (b1 == ENTITY_TYPE::PLAYER && b2 == ENTITY_TYPE::VIEWBOX)
				{
					Asset* asset = (*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pAs;
					ViewBox* viewbox = static_cast<ViewBox*>((*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key].pEn);
					viewbox->colliding = 1;
					asset->obj->SetDiffuse(1, 0, 0);
					//(*MemoryManager::GetInstance()->ObjectPool)[c->mpBodies[1]->key+1].pLd->pLt->SetDiffuse(1, 0, 0);
				}
			}
		}
		if (proxy_check == 1)
		{
			break;
		}
		if (abs((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_x) < 1 && abs((*MemoryManager::GetInstance()->ObjectPool)[2].pBd->mVelo_y) < 1)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[2].pCr->usable = true;
		}
	}

	for (unsigned int i = 0; i < n; ++i)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd)
		{
			if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->body_State == ON_SURFACE)
			{
				(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mLeftMovement = true;
				(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mRightMovement = true;
				(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mUpMovement = true;
				(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mDownMovement = true;
			}
		}
	}

	//CollisionManager::GetInstance()->ResetGrid();
}


void PhysicsManager::Destroy()
{
	delete mInstance;
}
