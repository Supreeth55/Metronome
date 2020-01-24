/**
 * @file GraphicsManager.h
 * @authors Ching-Yen Lin.
 * @date 9/1/2019
 * @brief This file has methods for rendering data on the screen
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
//#define _HAS_STD_BYTE 0
#include "../Source/pch.h"
#include "../Source/Graphics/DirectXHandler.h"
#include "../Source/Graphics/DXShader.h"
#include "../Source/Graphics/Camera.h"
#include "../Source/Graphics/DXObject.h"
#include "../Source/Graphics/DXTexture.h"
#include "../Source/Graphics/Light.h"
#include "../Manager/GameObjectManager.h"
#include "../Source/Graphics/DeferredRendering/DeferredRendering.h"


using namespace std;

struct WorldMatrixBuffer
{
	Matrix WorldView;
	Matrix WorldProj;
	Matrix WorldInverse;
};
class Asset;
class Body;
class GraphicsManager
{
public:
	/**
	all pointer point to null and set defualt value
	*/
	GraphicsManager();

	/**
	release pointers
	*/
	~GraphicsManager();

	/**
	Initialize directX, shader, buffer and anything need
	*/
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	/**
	release all pointer
	*/
	void Shutdown();

	/**
	Render all the things
	*/
	bool Update(float rdeltaTime);

	/**
	Resize screen
	*/
	void Resize(int screenWidth, int screenHeight);

	/**
	set debug mode
	*/
	void SetDebug(bool);

	/**
	get video card's name
	*/
	string GetDeviceName();

	/**
	get DirectXHandler
	*/
	DirectXHandler*GetDirectXHandler();

	/**
	get Manager itself
	*/
	static GraphicsManager* GetInstance();

	/**
	delete Manager itself
	*/
	void Destroy();

	/**
	Set Camera Rotation
	*/
	void SetCameraRotation(float, float, float, float);

	/**
	Set Camera Movement
	*/
	void SetCameraMovement(float, float, float, float);

	/**
	Set the distance between target and camera position
	*/
	void SetCameraDistance(float);

	/**
	Get camera position
	*/
	void GetCameraPosition(Vector3&);

	/**
	Get camera pointer
	*/
	Camera* GetCamera();



	//https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
	//for string to wstring

	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_FAR = 5000.0f;
	const float SCREEN_NEAR = 0.5f;
private:
	DXObject* pQuad;
	DXObject* pDisk;
	DXObject* pSkyDome;
	static GraphicsManager* mInstance;

	/**
	Render sence
	*/
	void Render();

	/**
	Make Matrix Buffer
	*/
	ID3D11Buffer* MakeMatrixBuffer(D3D11_BUFFER_DESC matrixBuffer);

	/**
	Draw Debug
	*/
	void DrawDebug();

	/**
	Draw Bones
	*/
	void DrawBones(Asset*);

	/**
	Draw Mouse
	*/
	void DrawMouse();

	/**
	Draw Bounding Box
	*/
	void DrawBoundingBox(Body*);

	/**
	Bind World Matrix to buffer
	*/
	void BindWorldMatrixBuffer(Matrix, Matrix);

	/**
	Bind Light to buffer
	*/
	void BindLightBuffer(Light*);

	HWND mHWND;
	int mScreenWidth, mScreenHeight;

	//get this pointer
	DirectXHandler* pDirectX;
	//Deferred Rander
	DeferredRendering mDeferredRendering;

	//shaders
	DXShader* pimplePShader;
	DXShader* pSimpleVShader;
	DXShader* pAnimedVShader;
	DXShader* pShadowPShader;


	//derred Vertex shader
	DXShader* pDeferredVShader;
	//derred pixel shader Gbuffer pass
	DXShader* pGbufferPShader;
	//derred pixel shader ambient pass
	DXShader* pDSAPShader;
	//derred pixel shader Light pass
	DXShader* pDSLPShader;


	ID3D11Buffer* pWorldMatrixBuff;
	ID3D11Buffer* pObjectBuff;
	ID3D11Buffer* pModelBuff;
	ID3D11Buffer* pLightBuff;
	ID3D11Buffer* pBoneBuff;

	Camera* pCamera;
	DXObject* pObject;

	DXTexture* pTexture;
	ID3D11SamplerState* pDefaultSamplerState;
	Light* pLight;

	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* pBrush;
	bool mDebugMode;

public:
	bool mPaused = false;
	bool mCompletion = false;
	bool mNeedCompletion = false;

};
#endif // !GRAPHICSMANAGER_H
