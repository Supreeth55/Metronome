/**
 * @file GraphicsManager.h
 * @authors Ching-Yen Lin.
 * @date 9/1/2019
 * @brief This file has methods for rendering data on the screen
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "GraphicsManager.h"
#include "ImGUIManager.h"
#include "../Source/Graphics/Shape.h"
#include "../Source/Components/Asset.h"
#include "../Source/Components/Transform.h"
#include "../Source/GameObject.h"
#include "../Manager/MemoryManager.h"
#include "../Manager/ImGUIManager.h"
#include "../Source/Components/Animation.h"
#include "../Source/Components/Body.h"
#include "../Manager/DXInputManager.h"
#include "../Source/Entity/LightList.h"


GraphicsManager::GraphicsManager()
{
	pDirectX = 0;
	pimplePShader = 0;
	pSimpleVShader = 0;
	pAnimedVShader = 0;
	pCamera = 0;
	pObject = 0;
	mScreenWidth = 0;
	mScreenHeight = 0;
	mDebugMode = false;
}
GraphicsManager::~GraphicsManager()
{
	pWorldMatrixBuff->Release();
	pObjectBuff->Release();
	pModelBuff->Release();
}
bool GraphicsManager::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	mHWND = hwnd;
	bool result;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	// Create the Direct3D object.
	pDirectX = new DirectXHandler();
	if (!pDirectX)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = pDirectX->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		//MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}
	pBrush = new PrimitiveBatch<VertexPositionColor>(pDirectX->GetDeviceContext());

	pSimpleVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pimplePShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pAnimedVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pShadowPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pDeferredVShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pGbufferPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pDSAPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pDSLPShader = new DXShader(pDirectX->GetDevice(), pDirectX->GetDeviceContext());

	pAnimedVShader->AddShader(StringtoWstring("AnimationVShader.cso").c_str(), VS, &hwnd);
	pSimpleVShader->AddShader(StringtoWstring("SimpleVShader.cso").c_str(), VS, &hwnd);
	pimplePShader->AddShader(StringtoWstring("SimplePShader.cso").c_str(), PS, &hwnd);
	pShadowPShader->AddShader(StringtoWstring("ShadowPShader.cso").c_str(), PS, &hwnd);

	pDeferredVShader->AddShader(StringtoWstring("DeferredVertexShader.cso").c_str(), VS, &hwnd);
	pGbufferPShader->AddShader(StringtoWstring("DefferedGbufferPShader.cso").c_str(), PS, &hwnd);
	pDSAPShader->AddShader(StringtoWstring("DeferredAmbientPShader.cso").c_str(), PS, &hwnd);
	pDSLPShader->AddShader(StringtoWstring("DefferedLightPShader.cso").c_str(), PS, &hwnd);
	vector<D3D11_INPUT_ELEMENT_DESC> AnimedSDLayout;
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION" ,                //SemanticName
			0,                          //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                          //InputSlot
			0,                          //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,//InputSlotClass
			0                           //InstanceDataStepRate
		});

	/* same as this
		D3D11_INPUT_ELEMENT_DESC temp;
		temp.SemanticName = "POSITION";
		temp.SemanticIndex = 0;
		temp.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		temp.InputSlot = 0;
		temp.AlignedByteOffset = 0;
		temp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		temp.InstanceDataStepRate = 0;
	*/
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"NORMAL" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TEXCOORD" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TANGENT" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"WEIGHTS" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32A32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	AnimedSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"BONEINDICES" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32A32_UINT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	pAnimedVShader->AddLayout(AnimedSDLayout);

	vector<D3D11_INPUT_ELEMENT_DESC> SimpleSDLayout;
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"POSITION" ,                //SemanticName
			0,                          //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                          //InputSlot
			0,                          //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,//InputSlotClass
			0                           //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"NORMAL" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TEXCOORD" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	SimpleSDLayout.push_back(
		D3D11_INPUT_ELEMENT_DESC{
			"TANGENT" ,                      //SemanticName
			0,                             //SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT,//Format
			0,                             //InputSlot
			D3D11_APPEND_ALIGNED_ELEMENT,  //AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,   //InputSlotClass
			0                              //InstanceDataStepRate
		});
	pSimpleVShader->AddLayout(SimpleSDLayout);
	pDeferredVShader->AddLayout(SimpleSDLayout);

	D3D11_BUFFER_DESC worldMatrixDesc;//register(b0)
	worldMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	worldMatrixDesc.ByteWidth = sizeof(WorldMatrixBuffer);
	worldMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	worldMatrixDesc.MiscFlags = 0;
	worldMatrixDesc.StructureByteStride = 0;
	pWorldMatrixBuff = MakeMatrixBuffer(worldMatrixDesc);

	D3D11_BUFFER_DESC ObjectDesc;//register(b1)
	ObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	ObjectDesc.ByteWidth = sizeof(ObjectInfo);
	ObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ObjectDesc.MiscFlags = 0;
	ObjectDesc.StructureByteStride = 0;
	pObjectBuff = MakeMatrixBuffer(ObjectDesc);

	D3D11_BUFFER_DESC ModelDesc;//register(b2)
	ModelDesc.Usage = D3D11_USAGE_DYNAMIC;
	ModelDesc.ByteWidth = sizeof(ModelTransformation);
	ModelDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ModelDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ModelDesc.MiscFlags = 0;
	ModelDesc.StructureByteStride = 0;
	pModelBuff = MakeMatrixBuffer(ModelDesc);

	D3D11_BUFFER_DESC lightDesc;//register(b3)
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.ByteWidth = sizeof(LightInfo);
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightDesc.MiscFlags = 0;
	lightDesc.StructureByteStride = 0;
	pLightBuff = MakeMatrixBuffer(lightDesc);

	D3D11_BUFFER_DESC boneDesc;//register(b4)
	boneDesc.Usage = D3D11_USAGE_DYNAMIC;
	boneDesc.ByteWidth = sizeof(BoneTransform);
	boneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	boneDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	boneDesc.MiscFlags = 0;
	boneDesc.StructureByteStride = 0;
	pBoneBuff = MakeMatrixBuffer(boneDesc);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = pDirectX->GetDevice()->CreateSamplerState(&samplerDesc, &pDefaultSamplerState);
	pDirectX->GetDeviceContext()->PSSetSamplers(0, 1, &pDefaultSamplerState);

	pCamera = new Camera();
	pCamera->SetPosition(0, -10, 1);
	mDeferredRendering.Initialize(mScreenWidth, mScreenHeight);

	/*OBJ* cube = new OBJ("./Resource/Model/Skeleton.obj");*/
	//OBJ* cube = new OBJ("./Resource/Model/cube.obj");
	//Box* q = new Box(VEC3(1,1,1), VEC3(-1,-1,-1));
	//Particles* particles = new Particles();
	//po = new DXObject(particles, mDirectX->GetDevice(), mDirectX->GetDeviceContext(),true);
	Quad * quad = new Quad();
	Disk * disk = new Disk(32);
	Sphere * sphere = new Sphere(32);
	//Triangle* q = new Triangle();
	/*mObject = new DXObject(cube, mDirectX->GetDevice(), mDirectX->GetDeviceContext());
	mObjectList.push_back(mObject);*/
	pQuad = new DXObject(quad, pDirectX->GetDevice(), pDirectX->GetDeviceContext());
	pDisk = new DXObject(disk, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), Matrix::CreateScale(100, 100, 1));
	/*mObject = new DXObject(quad, mDirectX->GetDevice(), mDirectX->GetDeviceContext());
	mObjectList.push_back(mObject);*/
	pTexture = new DXTexture();
	pTexture->Initialize(pDirectX->GetDevice(), pDirectX->GetDeviceContext(), wstring(L"./Resource/Image/sky.jpg").c_str());
	//mTexture->Use(NORMAL_TEXTURE_SLOT_BEGIN);
	pSkyDome = new DXObject(sphere, pDirectX->GetDevice(), pDirectX->GetDeviceContext(), Matrix::CreateScale(5000, 5000, 5000));
	pSkyDome->SetMaterialID(1);
	pSkyDome->SetTexture(DIFFUSE_TEXTURE, pTexture);
	/*FBX * s = new FBX("./Resource/Model/gh_sample_animation.fbx");


	mObjectList.push_back(new DXObject(s, mDirectX->GetDevice(), mDirectX->GetDeviceContext()));*/
	pLight = new Light();
	//direct light example
	pLight->SetPosition(0, 0.0f, 300.0f);
	pLight->SetDirection(0.0f, 0.0f, -1.0f);
	pLight->SetDiffuse(0.5f, 0.5f, 0.5f);
	pLight->SetAmbient(0.1f, 0.1f, 0.1f);
	pLight->SetSpecular(0.6f, 0.6f, 0.6f);
	pLight->SetType(DIRECT_LIGHT);
	pLight->HasShadow(true);
	pLight->ShadowMap().Initialize(1024, 1024);

	//InitLocalLights();
	//point light example
	//mLight->setPosition(0.0f, -3.0f, 0.0f);
	//mLight->setDiffuse(1.0f, 1.0f, 1.0f);
	//mLight->setAmbient(0.2f, 0.2f, 0.2f);
	//mLight->setSpecular(1.0f, 1.0f, 1.0f);
	//mLight->setType(POINT_LIGHT);
	//mLight->setAttenuation(1.0f,0.09f,0.032f);
	//spot light
	//mLight->setPosition(0.0f, -3.0f, 0.0f);
	//mLight->setDirection(0.0f, 1.0f, 0.0f);
	//mLight->setDiffuse(1.0f, 1.0f, 1.0f);
	//mLight->setAmbient(0.2f, 0.2f, 0.2f);
	//mLight->setSpecular(1.0f, 1.0f, 1.0f);
	//mLight->setAttenuation(1.0f, 0.09f, 0.032f);
	//mLight->setCutOff(12.5f, 17.5f);
	//mLight->setType(SPOT_LIGHT);


	return true;
}
void GraphicsManager::Shutdown()
{
	if (pDirectX)
	{
		pDirectX->Shutdown();
		delete pDirectX;
		pDirectX = 0;
	}
	if (pBrush)
	{
		delete pBrush;
		pBrush = 0;
	}
	if (pObject)
	{
		pObject->~DXObject();
	}
	if (pCamera)
	{
		pCamera->~Camera();
	}
	if (pimplePShader)
	{
		pimplePShader->~DXShader();
	}
	if (pSimpleVShader)
	{
		pSimpleVShader->~DXShader();
	}
	if (pAnimedVShader)
	{
		pAnimedVShader->~DXShader();
	}
	if (pShadowPShader)
	{
		pShadowPShader->~DXShader();
	}
	RELEASE_COM(pDefaultSamplerState);
	return;
}

bool GraphicsManager::Update(float rdeltaTime)
{
	//bool result;
	// Render the graphics scene.
	Render();
	/*result = Render();
	if (!result)
	{
		return false;
	}*/

	return true;
}

void GraphicsManager::Resize(int screenWidth, int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mDeferredRendering.Resize(screenWidth, screenHeight);
	pDirectX->ResizeScreen(screenWidth, screenHeight);
}

void GraphicsManager::SetDebug(bool b)
{
	mDebugMode = b;
}

ID3D11Buffer* GraphicsManager::MakeMatrixBuffer(D3D11_BUFFER_DESC matrixBuffer)
{
	HRESULT result;
	ID3D11Buffer* buff;

	result = pDirectX->GetDevice()->CreateBuffer(&matrixBuffer, NULL, &buff);

	if (FAILED(result))
	{
		return nullptr;
	}
	return buff;
}

void GraphicsManager::DrawDebug()
{
	BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
		{ Vector3(1),120,Vector3(1),-1 });
	BindModelTransformation(pDirectX->GetDeviceContext(), pModelBuff,
		Matrix());
	pSimpleVShader->Use();
	//mSimplePShader->Use();
	//mDirectX->EnableZBuffer(false);
	auto asd = std::vector<Matrix>();
	BindBoneTransformation(pDirectX->GetDeviceContext(), pBoneBuff, asd);
	pBrush->Begin();
	float divisions = 20.0f;
	float halfdiv = divisions / 2.0f;
	Vector4 xaxis(1.f, 0.f, 0.f, 1.0);
	Vector4 yaxis(0.f, 1.f, 0.f, 1.0);
	Vector4 scale = xaxis * (-divisions / 2.0f);
	for (long i = 0; i <= divisions; ++i)
	{
		VertexPositionColor v1(scale - yaxis * halfdiv, Colors::White);
		VertexPositionColor v2(scale + yaxis * halfdiv, Colors::White);
		v1.position.z = 0.01f;
		v2.position.z = 0.01f;
		scale = scale + xaxis;
		pBrush->DrawLine(v1, v2);
	}
	scale = yaxis * (-divisions / 2);
	for (long i = 0; i <= divisions; i++)
	{

		VertexPositionColor v1(scale - xaxis * halfdiv, Colors::White);
		VertexPositionColor v2(scale + xaxis * halfdiv, Colors::White);
		v1.position.z = 0.01f;
		v2.position.z = 0.01f;
		scale = scale + yaxis;
		pBrush->DrawLine(v1, v2);
	}
	pBrush->End();
	//mDirectX->EnableZBuffer(true);

}
void GraphicsManager::DrawBones(Asset* rpasset)
{
	pSimpleVShader->Use();
	FBX* tempFBX = dynamic_cast<FBX*>(rpasset->obj->pShape);
	BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
		{ Vector3(0,1,0),120,Vector3(1),-1 });
	BindModelTransformation(pDirectX->GetDeviceContext(), pModelBuff,
		rpasset->obj->GetObjectTr());
	pBrush->Begin();
	rpasset->obj->DrawBoneHelper(tempFBX->mRootBone, pBrush);
	pBrush->End();
}

//uncomplete
void GraphicsManager::DrawMouse()
{
	//Matrix WorldView, WorldProj;
	//mCamera->GetViewMatrix(WorldView);
	//mDirectX->GetProjectionMatrix(WorldProj);
	//mDirectX->EnableZBuffer(false);
	//mSimpleVShader->Use();
	//BindObjectInfo(mDirectX->GetDeviceContext(), mObjectBuff,
	//	{ Vector3(0,1,0),120,Vector3(1),-1 });
	////BindModelTransformation(mDirectX->GetDeviceContext(), mModelBuff,
	////	Matrix());

	//BindWorldMatrixBuffer(WorldView, WorldProj);
	//int x = 0, y = 0;

	//DXInputManager::GetInstance()->GetMouseLocation(x,y);
	//float pointX = (( 2.0*(float)x) / (float)mScreenWidth) - 1.0f;
	//float pointY = (((2.0*(float)y) / (float)mScreenHeight)-1.0f)*-1.0f;

	//auto center = Vector4(x / WorldProj._11, 0, y / WorldProj._22, 1);
	//Vector4 p1(center + Vector3(.1, 0, 0));
	//Vector4 p2(center + Vector3(-.1, 0, 0));
	//Vector4 p3(center + Vector3(0, 0, .1));
	//Vector4 p4(center + Vector3(0, 0, -.1));

	//mBrush->Begin();
	//VertexPositionColor v1(center, Colors::White);
	//VertexPositionColor v2(p1, Colors::White);
	//mBrush->DrawLine(v1, v2);
	//v2 = VertexPositionColor(p2, Colors::White);
	//mBrush->DrawLine(v1, v2);
	//v2 = VertexPositionColor(p3, Colors::White);
	//mBrush->DrawLine(v1, v2);
	//v2 = VertexPositionColor(p4, Colors::White);
	//mBrush->DrawLine(v1, v2);
	//mBrush->End();
	//mDirectX->EnableZBuffer(true);

}
void GraphicsManager::DrawBoundingBox(Body * rbody)
{
	pSimpleVShader->Use();
	BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
		{ Vector3(1,0,0),120,Vector3(1),-1 });
	if (rbody->objectSelected) 
	{
		BindObjectInfo(pDirectX->GetDeviceContext(), pObjectBuff,
			{ Vector3(0,0,1),120,Vector3(1),-1 });
	}
	
	BindModelTransformation(pDirectX->GetDeviceContext(), pModelBuff,
		Matrix());
	auto center = Vector4(rbody->mPos_x, rbody->mPos_y, rbody->mPos_z, 1);
	auto shapex = static_cast<ShapeAABB*>(rbody->mShape);
	Vector4 p1(center + Vector3(shapex->mLeft, shapex->mIn, shapex->mTop));
	Vector4 p2(center + Vector3(shapex->mLeft, shapex->mIn, shapex->mBottom));
	Vector4 p3(center + Vector3(shapex->mLeft, shapex->mOut, shapex->mTop));
	Vector4 p4(center + Vector3(shapex->mLeft, shapex->mOut, shapex->mBottom));
	Vector4 p5(center + Vector3(shapex->mRight, shapex->mIn, shapex->mTop));
	Vector4 p6(center + Vector3(shapex->mRight, shapex->mIn, shapex->mBottom));
	Vector4 p7(center + Vector3(shapex->mRight, shapex->mOut, shapex->mTop));
	Vector4 p8(center + Vector3(shapex->mRight, shapex->mOut, shapex->mBottom));


	pBrush->Begin();
	VertexPositionColor v1(p1, Colors::White);
	VertexPositionColor v2(p2, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p3, Colors::White);
	v2 = VertexPositionColor(p4, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p1, Colors::White);
	v2 = VertexPositionColor(p3, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p2, Colors::White);
	v2 = VertexPositionColor(p4, Colors::White);
	pBrush->DrawLine(v1, v2);

	v1 = VertexPositionColor(p1, Colors::White);
	v2 = VertexPositionColor(p5, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p2, Colors::White);
	v2 = VertexPositionColor(p6, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p3, Colors::White);
	v2 = VertexPositionColor(p7, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p4, Colors::White);
	v2 = VertexPositionColor(p8, Colors::White);
	pBrush->DrawLine(v1, v2);

	v1 = VertexPositionColor(p5, Colors::White);
	v2 = VertexPositionColor(p6, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p7, Colors::White);
	v2 = VertexPositionColor(p8, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p5, Colors::White);
	v2 = VertexPositionColor(p7, Colors::White);
	pBrush->DrawLine(v1, v2);
	v1 = VertexPositionColor(p6, Colors::White);
	v2 = VertexPositionColor(p8, Colors::White);
	pBrush->DrawLine(v1, v2);

	pBrush->End();
}
//bind matrix buffer by using map
void GraphicsManager::BindWorldMatrixBuffer(Matrix view, Matrix proj)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WorldMatrixBuffer* dataPtr;
	result = pDirectX->GetDeviceContext()->Map(pWorldMatrixBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;

	dataPtr = (WorldMatrixBuffer*)mappedResource.pData;
	dataPtr->WorldView = view.Transpose();
	dataPtr->WorldProj = proj.Transpose();
	dataPtr->WorldInverse = view.Transpose().Invert();
	pDirectX->GetDeviceContext()->Unmap(pWorldMatrixBuff, 0);
	pDirectX->GetDeviceContext()
		->VSSetConstantBuffers(0, 1, &(pWorldMatrixBuff));
}

void GraphicsManager::BindLightBuffer(Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightInfo* dataPtr;
	result = pDirectX->GetDeviceContext()->Map(pLightBuff
		, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))return;

	dataPtr = (LightInfo*)mappedResource.pData;
	*dataPtr = light->GetInfo();
	pDirectX->GetDeviceContext()->Unmap(pLightBuff, 0);
	pDirectX->GetDeviceContext()
		->PSSetConstantBuffers(3, 1, &(pLightBuff));
}


string GraphicsManager::GetDeviceName()
{

	char temp[128];
	int mem;
	pDirectX->GetVideoCardInfo(temp, mem);
	return	string(temp);
}

DirectXHandler * GraphicsManager::GetDirectXHandler()
{
	return pDirectX;
}



GraphicsManager * GraphicsManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new GraphicsManager();
	}
	return mInstance;
}

void GraphicsManager::Destroy()
{
	delete mInstance;
}

void GraphicsManager::SetCameraRotation(float x, float y, float z, float sensitivity)
{
	if (pCamera)
	{
		float pan, tile, roll;
		pCamera->GetPanTiltRoll(pan, tile, roll);
		pCamera->SetPan(pan - x / sensitivity);
		pCamera->SetTilt(tile - y / sensitivity);
		pCamera->SetRoll(roll - z / sensitivity);
	}
}


//NEED TODO: make it better like actual camera movemont
void GraphicsManager::SetCameraMovement(float x, float y, float z, float sensitivity)
{
	if (pCamera)
	{
		pCamera->SetMovement(x / sensitivity, y / sensitivity, z / sensitivity);
	}
}

void GraphicsManager::SetCameraDistance(float rdis)
{
	if (pCamera)
	{
		pCamera->mZoom += rdis;
		//std::cout << "Z " << mCamera->mZoom << std::endl;
		pCamera->SetMovement(0, 0, 0);
	}
}

void GraphicsManager::GetCameraPosition(Vector3 & rcamerapos)
{
	float x, y, z;

	pCamera->GetPosition(x,y,z);
	rcamerapos = Vector3(x, y, z);
}
Camera* GraphicsManager::GetCamera()
{
	return pCamera;
}



void GraphicsManager::Render()
{
	//Initial graphics stuff, removed from loop because why put it in a loop to slow down stuff

	// Clear the buffers to begin the scene.
	pDirectX->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	unsigned int key1 = MemoryManager::GetInstance()->current_position;
	for (unsigned int i = 0; i < key1; i++)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->mEntityName == "Player")
		{
			auto x = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionX;
			auto y = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionY;
			auto z = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr->mPositionZ;
			pCamera->SetPosition(x,y, z+1);
			pCamera->mTx = -x;
			pCamera->mTy = -y;
			break;
		}
	}

//set the camera
Matrix WorldView, WorldProj, light_view, light_pro;
Matrix Identity;

//mCamera->GetViewMatrix(WorldView);
pCamera->Get3rdPersonView(WorldView);
pLight->GetLightViewMatrix(light_view);
pLight->GetLightProjectionMatrix(light_pro);
pDirectX->GetProjectionMatrix(WorldProj);
BindWorldMatrixBuffer(WorldView, WorldProj);

//use Render target for Deferred Render
mDeferredRendering.Use(0.5f, 0.5f, 0.5f, 1.0f);
//remember open mDepth testing
pDirectX->EnableZBuffer(true);
//remember close Alpha Blending until there is Transparent object
pDirectX->SetAlphaBlending(NO_BLEND);
//use GBuffer pixel shader for drawing in Render target
pGbufferPShader->Use();
pSimpleVShader->Use();
//Matrix posn = Matrix::CreateTranslation(Vector3(-5,-3,0)).Transpose();
//po->SetObjectTr(posn);
//po->Draw(mObjectBuff, mModelBuff, mBoneBuff);

unsigned int key = MemoryManager::GetInstance()->current_position;
vector<int> lightKey;

for (unsigned int i = 0; i < key; i++)
{
	Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[i].pAs;
	Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
	Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[i].pAn;
	if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::SPOT_LIGHT ||
		(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::POINT_LIGHT ||
		(*MemoryManager::GetInstance()->ObjectPool)[i].pEn->GetEntityType() == ENTITY_TYPE::DIRECT_LIGHT)
	{
		lightKey.push_back(i);
	}
	if (pAs && pTr)
	{
		//use the shader
		if (pAn)
		{
			pAnimedVShader->Use();
			FBX* tempFBX = dynamic_cast<FBX*>(pAs->obj->pShape);
			tempFBX->Update(pAn->GetTotalTime(), pAn->GetCurrentStatus(), pAn->IsLoop(), pAn->GetSpeed());
		}
		else
		{
			pSimpleVShader->Use();
		}

		pAs->obj->SetObjectTr(pTr);//set position

		pAs->obj->Draw(pObjectBuff, pModelBuff, pBoneBuff);
	}
}
pSimpleVShader->Use();
pDisk->Draw(pObjectBuff, pModelBuff, pBoneBuff);
pSkyDome->Draw(pObjectBuff, pModelBuff, pBoneBuff);
if (mDebugMode)
{
	for (unsigned int i = 0; i < key; i++)
	{
		Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[i].pAs;
		Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[i].pAn;
		Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[i].pBd;
		if (pAs&& pTr)
		{
			if (pAn)
			{
				FBX* tempFBX = dynamic_cast<FBX*>(pAs->obj->pShape);
				tempFBX->Update(pAn->GetTotalTime(), pAn->GetCurrentStatus(), pAn->IsLoop(), pAn->GetSpeed());
				pDirectX->EnableZBuffer(false);
				//DrawBones(pAs);
				pDirectX->EnableZBuffer(true);
			}
			if (pBd)
			{
				pDirectX->EnableZBuffer(false);
				DrawBoundingBox(pBd);
				pDirectX->EnableZBuffer(true);
			}
		}
	}
	DrawDebug();
	DrawMouse();
}

//unuse Deferred Rendering
mDeferredRendering.Unuse();

pShadowPShader->Use();

//use the light
//should loop through light list
for (int i = 0; i < lightKey.size(); ++i)
{
		int index = lightKey[i];
		LightList* plight = static_cast<LightList*>(
			(*MemoryManager::GetInstance()->ObjectPool)[index].pEn);
		Light* pLt = plight->pLd->pLt;
		if ((*MemoryManager::GetInstance()->ObjectPool)[index].pEn->GetEntityType() == ENTITY_TYPE::DIRECT_LIGHT)
		{
			pLt->SetPosition((*MemoryManager::GetInstance()->ObjectPool)[2].pTr->mPositionX,
			(*MemoryManager::GetInstance()->ObjectPool)[2].pTr->mPositionY,
				pLt->GetInfo().position.z);
		}


		if (!pLt->HasShadow()) continue;

		BindLightBuffer(pLt);

		pLt->ShadowMap().StartDrawShadow();
		pLt->GetLightViewMatrix(light_view);
		pLt->GetLightProjectionMatrix(light_pro);

		BindWorldMatrixBuffer(light_view, light_pro);
		for (unsigned int i = 0; i < key; i++)
		{
			Asset* pAs = (*MemoryManager::GetInstance()->ObjectPool)[i].pAs;
			Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[i].pTr;
			Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[i].pAn;
			if (pAs && pTr)
			{
				//use the shader
				if (pAn)
				{
					FBX* tempFBX = dynamic_cast<FBX*>(pAs->obj->pShape);
					tempFBX->Update(pAn->GetTotalTime(), pAn->GetCurrentStatus(), pAn->IsLoop(), pAn->GetSpeed());
					pAnimedVShader->Use();
				}
				else
				{
					pSimpleVShader->Use();
				}

				pAs->obj->SetObjectTr(pTr);//set position

				pAs->obj->Draw(pObjectBuff, pModelBuff, pBoneBuff);
			}
		}
		pSimpleVShader->Use();
		pDisk->Draw(pObjectBuff, pModelBuff, pBoneBuff);
		pLt->ShadowMap().FinishDrawShadow();
	}



	//use GBuffer data
	mDeferredRendering.BindGBuffer(GBUFFER_TEXTURE_SLOT_BEGIN);

	//use vertex for Deferred Rendering
	//only draw quad
	BindWorldMatrixBuffer(WorldView, WorldProj);
	pDeferredVShader->Use();

	//draw Ambient pass for Deferred Rendering
	pDSAPShader->Use();
	pDirectX->SetAlphaBlending(NO_BLEND);
	pQuad->Draw(pObjectBuff, pModelBuff, pBoneBuff);

	//use the light
	//should loop through light list
	for (int i = 0; i < lightKey.size(); ++i)
	{
		int index = lightKey[i];
		LightList* plight = static_cast<LightList*>(
			(*MemoryManager::GetInstance()->ObjectPool)[index].pEn);
		Light* pLt = plight->pLd->pLt;
		BindLightBuffer(pLt);

		if (pLt->HasShadow())
		{
			pLt->ShadowMap().BindShadowMap(SHADOW_TEXTURE_SLOT_BEGIN);
		}
		////draw Light pass for Deferred Rendering
		pDSLPShader->Use();
		pDirectX->EnableZBuffer(false);
		pDirectX->SetAlphaBlending(ONE_ONE_BLEND);
		pDirectX->SetRasterizer(BACK_CULL);
		pQuad->Draw(pObjectBuff, pModelBuff, pBoneBuff);

		pDirectX->SetRasterizer(NO_CULL);
		if (pLt->HasShadow())
		{
			pLt->ShadowMap().UnBindShadowMap(SHADOW_TEXTURE_SLOT_BEGIN);
		}
	}
	mDeferredRendering.UnBindGBuffer(GBUFFER_TEXTURE_SLOT_BEGIN);

	if (mDebugMode)
	{
		ImGUIManager::GetInstance()->DrawWindows();
	}
	if (mPaused)
	{
		ImGUIManager::GetInstance()->DrawCharStats();
	}
	if (mCompletion)
	{
		ImGUIManager::GetInstance()->DrawCompletion();
	}
	if(mNeedCompletion)
	{
		ImGUIManager::GetInstance()->NeedCompletion();
	}
	
	mNeedCompletion = false;
	//render on the screen
	//should not draw anything after this
	//ImGUIManager::GetInstance()->DrawWindows();
	pDirectX->EndScene();

}
