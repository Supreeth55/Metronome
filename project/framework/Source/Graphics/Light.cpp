/**
 * @file Light.cpp
 * @author Ching-Yen Lin
 * @date 10/10/19
 * @brief This file content Light function like set member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Light.h"
#include "..\..\Manager\GraphicsManager.h"
#include "../Util/Math.h"

 /**
initial all member to zero
 */
Light::Light()
{
	mHasShadow = false;
	mInfo = { Vector4(),Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Vector3(),0,
			Matrix()};
	mInfo.shadowMartix._44 = -1.0f;
}
/**
copy data
 */
Light::Light(LightInfo info)
{
	mInfo = info;
	mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}


Light::~Light()
{
}
/**
Set Position
 */
void Light::SetPosition(float x, float y, float z)
{
	mInfo.position.x = x;
	mInfo.position.y = y;
	mInfo.position.z = z;
	mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}
/**
Set Direction
 */
void Light::SetDirection(float dirX, float dirY, float dirZ)
{
	mInfo.direction.x= dirX;
	mInfo.direction.y= dirY;
	mInfo.direction.z= dirZ;
	mLightViewMatrix = Matrix::CreateLookAt(mInfo.position, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f));
}
/**
Set Ambient
 */
void Light::SetAmbient(float r, float g, float b)
{
	mInfo.ambient.x = r;
	mInfo.ambient.y = g;
	mInfo.ambient.z = b;
}
/**
Set Diffuse
 */
void Light::SetDiffuse(float r, float g, float b)
{
	mInfo.diffuse.x = r;
	mInfo.diffuse.y = g;
	mInfo.diffuse.z = b;
}
/**
Set Specular
 */
void Light::SetSpecular(float r, float g, float b)
{
	mInfo.specular.x = r;
	mInfo.specular.y = g;
	mInfo.specular.z = b;
}
/**
Set Attenuation
 */
void Light::SetAttenuation(float c, float l, float q)
{
	mInfo.constant = c;
	mInfo.linear = l;
	mInfo.quadratic = q;
}
/**
Set CutOff
 */
void Light::SetCutOff(float cut, float othercut)
{
	mInfo.cutOff = cos(cut* DEGREE_TO_RADIAN);
	mInfo.outerCutOff = cos(othercut* DEGREE_TO_RADIAN);
}
/**
Set Enable
 */
void Light::SetEnable(bool enable)
{
	mInfo.attribute.y = enable;
}
/**
Set Type
 */
void Light::SetType(LIGHTTYPE type)
{
	mInfo.attribute.x = float(type);
}
/**
Get Light View Matrix
 */
void Light::GetLightViewMatrix(Matrix & rMatrix)
{
	Vector3 up = PerpendicularFinder(mInfo.direction);
	Matrix L = MyLookAt(mInfo.position, mInfo.direction,up);
	rMatrix = L;
}
/**
Get Light Projection Matrix
 */
void Light::GetLightProjectionMatrix(Matrix & rMatrix)
{
	float light_dist = 500.0f;
	//auto dgfrMatrix = Matrix::CreatePerspectiveFieldOfView(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.0f);
	MAT4 lightProjection = Perspective(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);
	Matrix asd=MyPerspective(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);
	rMatrix = asd;
}
/**
Initialize Shadow Map
 */
void Light::ShadowMap(float rwidth, float rheight)
{
	mShadowMap.Initialize(rwidth, rheight);
}
/**
get Shadow Map
 */
Shadow & Light::ShadowMap()
{
	return mShadowMap;
}
/**
set hasShadow
 */
void Light::HasShadow(bool rhasshadow)
{
	mHasShadow = rhasshadow;
}
/**
get HasShadow
 */
bool Light::HasShadow()
{
	return mHasShadow;
}
/**
Get light Info
 */
LightInfo& Light::GetInfo()
{
	if (mHasShadow)
	{
		ComputerShadowMatrix();
	}
	return mInfo;
}
/**
Computer Shadow Matrix
 */
void Light::ComputerShadowMatrix()
{
	auto screen_aspect = GraphicsManager::GetInstance()->GetDirectXHandler()->GetAspectRatio();
	float light_dist = 500.0;
	
	Vector3 up = PerpendicularFinder(mInfo.direction + mInfo.position);
	Matrix p = MyPerspective(40.0f / light_dist, 40.0f / light_dist, 0.1f, 1000.f);
	Matrix l = MyLookAt(mInfo.position, mInfo.direction,up);
	mInfo.shadowMartix = Matrix(Matrix::CreateTranslation(0.5,0.5,0.5).Transpose()*
		Matrix::CreateScale(0.5, 0.5, 0.5)*p.Transpose()*l.Transpose());
}
