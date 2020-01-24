/**
 * @file CollisionManager.cpp
 * @author Vineet Dogra, Romil Tendulkar
 * @date 09/26/2019
 * @brief This class implements the various collision detection functions that are needed
		As well as the data structure needed for spatial partitioning
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "CollisionManager.h"
#include "..\Source\Util\ShapeX.h"
#include <math.h>


Contact::Contact()
{
	mpBodies[0] = mpBodies[1] = nullptr;
}

Contact::~Contact()
{

}

CollisionManager* CollisionManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new CollisionManager();
	}
	return mInstance;
}

CollisionManager::CollisionManager()
{
	mSpatial = new SpatialGrid(100, 16);
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Reset()
{
	//std::cout << "number of freed collisions: " << mContacts.size() << std::endl;
	for (Contact* c : mContacts)
	{
		delete c;
	}

	mContacts.clear();
}

bool CollisionManager::CheckCollisionCuboidCuboid(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y,
	float pos2z)
{

	ShapeAABB* pAABB1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);
	

	// Get the half dimension values for each cube
	float halfWidth1 = pAABB1->pOwnerBody->mScale_x;
	float halfWidth2 = pAABB2->pOwnerBody->mScale_x;

	float halfHeight1 = pAABB1->pOwnerBody->mScale_y;
	float halfHeight2 = pAABB2->pOwnerBody->mScale_y;

	float halfDepth1 = pAABB1->pOwnerBody->mScale_z;
	float halfDepth2 = pAABB2->pOwnerBody->mScale_z;

	// Update positions with possible offsets
	float xOffset1 = ((pAABB1->mLeft + pAABB1->mRight) / 2.0f);
	float xOffset2 = ((pAABB2->mLeft + pAABB2->mRight) / 2.0f);
	float yOffset1 = ((pAABB1->mBottom + pAABB1->mTop) / 2.0f);
	float yOffset2 = ((pAABB2->mBottom + pAABB2->mTop) / 2.0f);

	Vector3x Ax, Ay, Az, Bx, By, Bz;
	Vector3x T;
	Vector3x PA, PB;
	Ax.x = cos(pAABB1->pOwnerBody->mAng_x);
	Ax.y = Ax.z = 0;
	Ay.y = cos(pAABB1->pOwnerBody->mAng_y);
	Ay.x = Ay.z = 0;
	Az.z = cos(pAABB1->pOwnerBody->mAng_z);
	Az.x = Az.y = 0;
	Bx.x = cos(pAABB2->pOwnerBody->mAng_x);
	Bx.y = Bx.z = 0;
	By.y = cos(pAABB2->pOwnerBody->mAng_y);
	By.x = By.z = 0;
	Bz.z = cos(pAABB2->pOwnerBody->mAng_z);
	Bz.x = Bz.y = 0;
	PA.x = pos1x;
	PA.y = pos1y;
	PA.z = pos1z;
	//////////////////////////////////////////////////////
	PB.x = pos2x;
	PB.y = pos2y;
	PB.z = pos2z;
	Vector3Zero(&T);
	Vector3Sub(&T, &PA, &PB);//GOT T

	//Case 1
	if (std::abs(Vector3DotProduct(&T, &Ax)) > (halfWidth1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Ax)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Ax)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Ax))))
	{
		return true;
	}

	//Case 2
	if (std::abs(Vector3DotProduct(&T, &Ay)) > (halfHeight1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Ay)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Ay)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Ay))))
	{
		return true;
	}

	//Case 3
	if (std::abs(Vector3DotProduct(&T, &Az)) > (halfDepth1 + std::abs(halfWidth2 * Vector3DotProduct(&Bx, &Az)) + std::abs(halfHeight2 * Vector3DotProduct(&By, &Az)) +
		std::abs(halfDepth2 * Vector3DotProduct(&Bz, &Az))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////
	//Case 4
	if (std::abs(Vector3DotProduct(&T, &Bx)) > (halfWidth2 + std::abs(halfWidth1 * Vector3DotProduct(&Bx, &Ax)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &Bx)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &Bx))))
	{
		return true;
	}

	//Case 5
	if (std::abs(Vector3DotProduct(&T, &Ay)) > (halfHeight2 + std::abs(halfWidth1 * Vector3DotProduct(&Ax, &By)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &By)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &By))))
	{
		return true;
	}

	//Case 6
	if (std::abs(Vector3DotProduct(&T, &Az)) > (halfDepth2 + std::abs(halfWidth1 * Vector3DotProduct(&Ax, &Bz)) + std::abs(halfHeight1 * Vector3DotProduct(&Ay, &Bz)) +
		std::abs(halfDepth1 * Vector3DotProduct(&Az, &Bz))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////
	//Case 7
	Vector3x AxBx;
	Vector3CrossProduct(&AxBx, &Ax, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AxBx)) > (std::abs(halfHeight1 * Vector3DotProduct(&Bx, &Az)) + std::abs(halfDepth1 * Vector3DotProduct(&Bx, &Ay)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Ax, &Bz)) + std::abs(halfDepth2 * Vector3DotProduct(&Ax, &By))))
	{
		return true;
	}

	//Case 8
	Vector3x AxBy;
	Vector3CrossProduct(&AxBy, &Ax, &By);
	if (std::abs(Vector3DotProduct(&T, &AxBy)) > (std::abs(halfHeight1 * Vector3DotProduct(&Az, &By)) + std::abs(halfDepth1 * Vector3DotProduct(&Ay, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Ax)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Ax))))
	{
		return true;
	}

	//Case 9
	Vector3x AxBz;
	Vector3CrossProduct(&AxBz, &Ax, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AxBz)) > (std::abs(halfHeight1 * Vector3DotProduct(&Az, &Bz)) + std::abs(halfDepth1 * Vector3DotProduct(&Ay, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&By, &Ax)) + std::abs(halfHeight2 * Vector3DotProduct(&Bx, &Ax))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////
	//Case10
	Vector3x AyBx;
	Vector3CrossProduct(&AyBx, &Ay, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AyBx)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &Bx)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &Bx)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Bz, &Ay)) + std::abs(halfDepth2 * Vector3DotProduct(&By, &Ay))))
	{
		return true;
	}

	//Case 11
	Vector3x AyBy;
	Vector3CrossProduct(&AyBy, &Ay, &By);
	if (std::abs(Vector3DotProduct(&T, &AyBy)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &By)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Ay)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Ay))))
	{
		return true;
	}

	//Case 12
	Vector3x AyBz;
	Vector3CrossProduct(&AyBz, &Ay, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AyBz)) > (std::abs(halfWidth1 * Vector3DotProduct(&Az, &Bz)) + std::abs(halfDepth1 * Vector3DotProduct(&Ax, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&By, &Ay)) + std::abs(halfHeight2 * Vector3DotProduct(&Bx, &Ay))))
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////
	//Case 13
	Vector3x AzBx;
	Vector3CrossProduct(&AzBx, &Az, &Bx);
	if (std::abs(Vector3DotProduct(&T, &AzBx)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &Bx)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &Bx)) +
		std::abs(halfHeight2 * Vector3DotProduct(&Bz, &Az)) + std::abs(halfDepth2 * Vector3DotProduct(&By, &Az))))
	{
		return true;
	}

	//Case 14
	Vector3x AzBy;
	Vector3CrossProduct(&AzBy, &Az, &By);
	if (std::abs(Vector3DotProduct(&T, &AzBy)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &By)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &By)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Bz, &Az)) + std::abs(halfDepth2 * Vector3DotProduct(&Bx, &Az))))
	{
		return true;
	}

	//Case 15
	Vector3x AzBz;
	Vector3CrossProduct(&AzBz, &Az, &Bz);
	if (std::abs(Vector3DotProduct(&T, &AzBz)) > (std::abs(halfWidth1 * Vector3DotProduct(&Ay, &Bz)) + std::abs(halfHeight1 * Vector3DotProduct(&Ax, &Bz)) +
		std::abs(halfWidth2 * Vector3DotProduct(&Az, &By)) + std::abs(halfHeight2 * Vector3DotProduct(&Az, &Bx))))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CollisionManager::CheckBroadPhase2(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z)
{
	ShapeAABB* pAABB1 = static_cast<ShapeAABB*>(pShape1);
	float bottom1 = pos1y - pAABB1->pOwnerBody->mScale_y;
	float top1 = pos1y + pAABB1->pOwnerBody->mScale_y;
	float in1 = pos1z + pAABB1->pOwnerBody->mScale_z;
	float out1 = pos1z - pAABB1->pOwnerBody->mScale_z;
	float left1 = pos1x - pAABB1->pOwnerBody->mScale_x;
	float right1 = pos1x + pAABB1->pOwnerBody->mScale_x;

	ShapeAABB* pAABB2 = static_cast<ShapeAABB*>(pShape2);
	float bottom2 = pos1y - pAABB2->pOwnerBody->mScale_y;
	float top2 = pos1y + pAABB2->pOwnerBody->mScale_y;
	float in2 = pos1z + pAABB2->pOwnerBody->mScale_z;
	float out2 = pos1z - pAABB2->pOwnerBody->mScale_z;
	float left2 = pos1x - pAABB2->pOwnerBody->mScale_x;
	float right2 = pos1x + pAABB2->pOwnerBody->mScale_x;

	if (bottom1 > top2 || top1 < bottom2)
	{
		return false;
	}
	if (left1 > right2 || right1 < left2)
	{
		return false;
	}
	if (out1 > in2 || in1 < out2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CollisionManager::ResetGrid()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mSpatial->Cells[i][j].data.clear();
		}
	}
}