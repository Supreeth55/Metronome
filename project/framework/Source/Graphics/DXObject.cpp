/**
 * @file DXObject.cpp
 * @author Ching-Yen Lin
 * @date 9/01/19
 * @brief This file content DXObject function like draw, initial and other setting function
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "DXObject.h"
#include "Shape.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/TimerManager.h"
 /**
  Initial the DXObject
 */
DXObject::DXObject(Shape* shape, ID3D11Device * device, ID3D11DeviceContext *deviceContext, bool rdynamicvertex)
	:pDeviceContext(deviceContext), pShape(shape), mObjectTr(Matrix())
{
	mDynamicVertex = rdynamicvertex;
	mObjectInfo.diffuse = shape->mDefaultDiffuseColor;
	mObjectInfo.specular = shape->mDefaultSpecularColor;
	mObjectInfo.shininess = shape->mDefaultShininess;
	mObjectInfo.MaterialID = shape->mDefaultMaterialID;

	pVertexBuffer = 0;
	pIndexBuffer = 0;
	//SimpleVertexType* vertices;
	//unsigned int* indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA vData, iData;
	HRESULT result;

	mVertexCount =(unsigned int) shape->mPntcount;
	mIndexCount =(unsigned int) shape->mTricount;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if (true == mDynamicVertex)
	{
		VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}
	if (pShape->mHasBone == true)
	{
		VertexBufferDesc.ByteWidth = sizeof(AnimationVertexType) * mVertexCount;
	}
	else
	{
		VertexBufferDesc.ByteWidth = sizeof(SimpleVertexType) * mVertexCount;
	}
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	if (true == mDynamicVertex)
	{
		VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	if (pShape->mHasBone == true)
	{
		vData.pSysMem = &pShape->LAnimedVertexData[0];
	}
	else
	{
		vData.pSysMem = &pShape->LSimpleVertexData[0];
	}

	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&VertexBufferDesc, &vData, &pVertexBuffer);

	if (FAILED(result))
	{
		return;
	}
	//D3D11_USAGE_DYNAMIC
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned int) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	iData.pSysMem = &(pShape->LFace[0]);
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&IndexBufferDesc, &iData, &pIndexBuffer);
	if (FAILED(result))
	{
		return;
	}


	return;

}
/**
 Initial the DXObject with define Object Transform
*/
DXObject::DXObject(Shape* shape, ID3D11Device * device, ID3D11DeviceContext *deviceContext, Matrix objectTr)
	:DXObject(shape, device, deviceContext)
{
	mObjectTr = objectTr;
}
/**
 draw object
*/
void DXObject::Draw(ID3D11Buffer* objectBuff, ID3D11Buffer* modelBuff, ID3D11Buffer* boneBuff)
{
	unsigned int stride;
	unsigned int offset;

	//bind object all information
	BindModelTransformation(pDeviceContext, modelBuff,
		mObjectTr*pShape->mModelTr);

	auto emptylist = std::vector<Matrix>();
	if (pShape->mHasAnimation == true)
	{
		BindBoneTransformation(pDeviceContext, boneBuff, pShape->LBoneTr);
	}
	else
	{
		BindBoneTransformation(pDeviceContext, boneBuff, emptylist);
	}
	// Set vertex buffer stride and offset.
	if (pShape->mHasBone)stride = sizeof(AnimationVertexType);
	else stride = sizeof(SimpleVertexType);
	offset = 0;
	if (mDynamicVertex)
	{
		Particles* pPs = static_cast<Particles*> (pShape);
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		SimpleVertexType* verticesPtr;
		result = pDeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		verticesPtr = (SimpleVertexType*)mappedResource.pData;
		pPs->Update(TimerManager::GetInstance()->getDeltaTime());
		vector<ParticleInfo> plist = pPs->GetParticlelist();
		Vector3 cp;
		GraphicsManager::GetInstance()->GetCameraPosition(cp);
		for(size_t i=0, index=0; i< pPs->mMaxParticles; i++)
		{
			if(plist[i].active==false)
			{
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				verticesPtr[index].vertex = Vector3(0, 0, 0);
				index++;
				continue;
			}

			Vector3 c = plist[i].position;
			Vector3 look = cp - c;
			//Matrix mView = Matrix::CreateBillboard(c, cp, Vector3::Up, Vector3::Backward);
			look.Normalize();
			Vector3 right = Vector3(0, 0.5, 0).Cross(look);
			right.Normalize();
			Vector3 up = look.Cross(right);
			right = right * 0.1f;
			up = up * 0.1f;
			// Bottom left.
			verticesPtr[index].vertex = c - right - up;
			verticesPtr[index].vertexTexture = Vector2(0, 0);
			index++;
			// Top left.
			verticesPtr[index].vertex = c - right + up;
			verticesPtr[index].vertexTexture = Vector2(0, 1);
			index++;
			// Bottom right.
			verticesPtr[index].vertex = c + right - up;
			verticesPtr[index].vertexTexture = Vector2(1, 0);
			index++;
			// Bottom right.
			verticesPtr[index].vertex = c + right - up;
			verticesPtr[index].vertexTexture = Vector2(1, 0);
			index++;
			// Top left.
			verticesPtr[index].vertex = c - right + up;
			verticesPtr[index].vertexTexture = Vector2(0, 1);
			index++;
			// Top right.
			verticesPtr[index].vertex = c + right + up;
			verticesPtr[index].vertexTexture = Vector2(1, 1);
			index++;
		}
		pDeviceContext->Unmap(pVertexBuffer, 0);
		mObjectInfo.MaterialID = 3;

	}
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (size_t i = 0; i < pShape->LSubMeshList.size(); i++)
	{
		if (mCustomColor != true)
		{
			SetTextureIO(pShape->LSubMeshList[i]);
		}
		BindObjectInfo(pDeviceContext, objectBuff, mObjectInfo);
		pDeviceContext->DrawIndexed(pShape->LSubMeshList[i].FaceCount, pShape->LSubMeshList[i].FaceStart, pShape->LSubMeshList[i].VertexStart);
		RestTextureIO(pShape->LSubMeshList[i]);
	}

	return;
}
/**
Draw Bone Helper for recursive call
*/
void DXObject::DrawBoneHelper(BoneNode & rootnode, PrimitiveBatch<VertexPositionColor>*Brush)
{

	FBX* tempFBX = dynamic_cast<FBX*>(pShape);
	//mShape->boneTr[rootnode.index];

	Vector4 xaxis(0.f, 0.f, 0.f, 1.0);
	Vector4 yaxis(0.f, 0.f, 0.f, 1.0);
	float x = pShape->LBonePnt[rootnode.mIndex]._14;
	float y = pShape->LBonePnt[rootnode.mIndex]._24;
	float z = pShape->LBonePnt[rootnode.mIndex]._34;
	VertexPositionColor v1(Vector4(x, y, z, 1), Colors::White);
	for (int i = 0; i < rootnode.LChildren.size(); i++)
	{
		float x1 = pShape->LBonePnt[rootnode.LChildren[i].mIndex]._14;
		float y1 = pShape->LBonePnt[rootnode.LChildren[i].mIndex]._24;
		float z1 = pShape->LBonePnt[rootnode.LChildren[i].mIndex]._34;
		VertexPositionColor v2(Vector4(x1, y1, z1, 1), Colors::White);
		Brush->DrawLine(v1, v2);
		DrawBoneHelper(rootnode.LChildren[i], Brush);
	}
}
/**
Set Texture
*/
void DXObject::SetTexture(TEXTURETYPE rtexturetype, DXTexture * rtexture)
{
	LTextureList[rtexturetype] = rtexture;
}
/**
Set Texture slots
*/
void DXObject::SetTextureIO(SubMesh& rsubmesh)
{
	mObjectInfo.TextureIO = XMUINT4(0, 0, 0, 0);
	if (LTextureList[DIFFUSE_TEXTURE])
	{
		LTextureList[DIFFUSE_TEXTURE]->Bind(COLOR_TEXTURE_SLOT_BEGIN);
		mObjectInfo.TextureIO.x = 1;
	}
	else if (rsubmesh.DiffuseTextureID)
	{
		rsubmesh.DiffuseTextureID->Bind(COLOR_TEXTURE_SLOT_BEGIN);
		mObjectInfo.TextureIO.x = 1;
	}
	else
	{
		mObjectInfo.diffuse = rsubmesh.DiffuseColor;
	}
	if (LTextureList[SPECULAR_TEXTURE])
	{
		LTextureList[SPECULAR_TEXTURE]->Bind(COLOR_TEXTURE_SLOT_BEGIN + 1);
		mObjectInfo.TextureIO.y = 1;
	}
	else if (rsubmesh.SpecularTextureID)
	{
		rsubmesh.SpecularTextureID->Bind(COLOR_TEXTURE_SLOT_BEGIN + 1);
		mObjectInfo.TextureIO.y = 1;
	}
	else
	{
		mObjectInfo.specular = rsubmesh.SpecularColor;
	}
	if (LTextureList[SHININESS_TEXTURE])
	{
		LTextureList[SHININESS_TEXTURE]->Bind(COLOR_TEXTURE_SLOT_BEGIN + 2);
		mObjectInfo.TextureIO.z = 1;
	}
	else if (rsubmesh.ShininessTextureID)
	{
		rsubmesh.ShininessTextureID->Bind(COLOR_TEXTURE_SLOT_BEGIN + 2);
		mObjectInfo.TextureIO.z = 1;
	}
	else
	{
		mObjectInfo.shininess = rsubmesh.Shininess;
	}
	if (LTextureList[NORMAL_TEXTURE])
	{
		LTextureList[NORMAL_TEXTURE]->Bind(COLOR_TEXTURE_SLOT_BEGIN + 3);
		mObjectInfo.TextureIO.w = 1;
	}
	else if (rsubmesh.NormalTextureID) {
		rsubmesh.NormalTextureID->Bind(COLOR_TEXTURE_SLOT_BEGIN + 3);
		mObjectInfo.TextureIO.w = 1;
	}
}
/**
reset Texture slots
*/
void DXObject::RestTextureIO(SubMesh & rsubmesh)
{
	mObjectInfo.TextureIO = XMUINT4(0, 0, 0, 0);
	if (rsubmesh.DiffuseTextureID)
		rsubmesh.DiffuseTextureID->UnBind(COLOR_TEXTURE_SLOT_BEGIN);
	if (rsubmesh.SpecularTextureID)
		rsubmesh.DiffuseTextureID->UnBind(COLOR_TEXTURE_SLOT_BEGIN + 1);
	if (rsubmesh.ShininessTextureID)
		rsubmesh.DiffuseTextureID->UnBind(COLOR_TEXTURE_SLOT_BEGIN + 2);
	if (rsubmesh.NormalTextureID)
		rsubmesh.DiffuseTextureID->UnBind(COLOR_TEXTURE_SLOT_BEGIN + 3);
}
/**
set Diffuse 
*/
void DXObject::SetDiffuse(float r, float g, float b)
{
	mCustomColor = true;
	mObjectInfo.diffuse.x = r;
	mObjectInfo.diffuse.y = g;
	mObjectInfo.diffuse.z = b;
}
/**
set Specular
*/
void DXObject::SetSpecular(float r, float g, float b)
{
	mCustomColor = true;
	mObjectInfo.specular.x = r;
	mObjectInfo.specular.y = g;
	mObjectInfo.specular.z = b;
}
/**
set Shininess
*/
void DXObject::SetShininess(float a)
{
	mCustomColor = true;
	mObjectInfo.shininess = a;
}
/**
set MaterialID
*/
void DXObject::SetMaterialID(int id)
{
	mObjectInfo.MaterialID = id;
}
/**
return bool about Has Bone
*/
bool DXObject::HasBone()
{
	return pShape->mHasBone;
}
/**
clean VBO
*/
DXObject::~DXObject()
{
	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}

}



void DXObject::SetObjectTr(Transform* pTr)
{
	//set the 4x4 SRT matrices here 
	Matrix S, Rx, Rz, T;
	S = Matrix::CreateScale(Vector3(pTr->mScaleX, pTr->mScaleY, pTr->mScaleZ));
	T = Matrix::CreateTranslation(Vector3(pTr->mPositionX, pTr->mPositionY, pTr->mPositionZ)).Transpose();

	if (pTr->mRotationX == 90)
		Rx = Matrix::CreateRotationX(pTr->mRotationX*DEGREE_TO_RADIAN).Transpose();

	//if (pTr->mRotationZ != 0)
	Rz = Matrix::CreateRotationZ(pTr->mRotationZ * DEGREE_TO_RADIAN).Transpose();
	
	mObjectTr = T*Rz*Rx*S;//find a way to add Rotation also
	//mObjectTr.m[0][0] = pTr->mScaleX;
	//mObjectTr.m[1][1] = pTr->mScaleY;
	//mObjectTr.m[2][2] = pTr->mScaleZ;
	//mObjectTr.m[0][3] = pTr->mPositionX; mObjectTr.m[1][3] = pTr->mPositionY; mObjectTr.m[2][3] = pTr->mPositionZ;

	//mObjectTr = ObjectTr;
}
void DXObject::SetObjectTr(Matrix ObjectTr)
{
	mObjectTr = ObjectTr;
}

Matrix & DXObject::GetObjectTr()
{
	return mObjectTr;
}



void BindObjectInfo(ID3D11DeviceContext * DeviceContext, ID3D11Buffer * objectBuff,
	ObjectInfo objectInfo)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//KD KS Alpha
	ObjectInfo* objPtr;
	result = DeviceContext->Map(objectBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;
	objPtr = (ObjectInfo*)mappedResource.pData;

	*objPtr = objectInfo;
	DeviceContext->Unmap(objectBuff, 0);
	DeviceContext->PSSetConstantBuffers(1, 1, &(objectBuff));
}

void BindModelTransformation(ID3D11DeviceContext * DeviceContext, ID3D11Buffer * modelBuff,
	Matrix objectTr)
{
	//add modeltrl
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ModelTransformation* modelPtr;
	unsigned int bufferNumber;
	result = DeviceContext->Map(modelBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;
	modelPtr = (ModelTransformation*)mappedResource.pData;
	modelPtr->ModelTr = objectTr;
	modelPtr->NormalTr = objectTr.Invert().Transpose();
	DeviceContext->Unmap(modelBuff, 0);
	bufferNumber = 1;
	DeviceContext->VSSetConstantBuffers(2, 1, &(modelBuff));
}
void BindBoneTransformation(ID3D11DeviceContext * DeviceContext, ID3D11Buffer * boneBuff, std::vector<Matrix>& boneTrList)
{
	//add modeltrl
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	BoneTransform* bonesPtr;
	result = DeviceContext->Map(boneBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;
	bonesPtr = (BoneTransform*)mappedResource.pData;
	bonesPtr->boneTr[0] = Matrix();
	for (size_t i = 0; i < boneTrList.size(); i++)
	{
		bonesPtr->boneTr[i] = boneTrList[i];
	}
	DeviceContext->Unmap(boneBuff, 0);
	DeviceContext->VSSetConstantBuffers(4, 1, &(boneBuff));
}
