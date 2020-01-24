/**
 * @file DXTexture.cpp
 * @author Ching-Yen Lin
 * @date 9/23/19
 * @brief This file content DXTexture member
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "DXTexture.h"
 /**
  Initial the DXTexture from file
 */
void DXTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filepath)
{
	pDeviceContext = deviceContext;
	HRESULT result = CreateWICTextureFromFile(device, filepath, nullptr, &pTexture);
}
/**
 Initial the DXTexture from memory
*/
void DXTexture::Initialize(ID3D11Device * device, ID3D11DeviceContext * devicecontext, const unsigned char * databuff, size_t buffsize)
{
	pDeviceContext = devicecontext;
	HRESULT result = CreateWICTextureFromMemory(device, databuff, buffsize, nullptr, &pTexture);
}

/**
clean menber
*/
DXTexture::~DXTexture()
{
	if (pTexture)
	{
		pTexture->Release();
		pTexture = 0;
	}
}
/**
Get Texture pointer
*/
ID3D11ShaderResourceView * DXTexture::GetTexture()
{
	return pTexture;
}
/**
use Texture with slot id
*/
void DXTexture::Bind(int textureID)
{
	pDeviceContext->PSSetShaderResources(textureID, 1, &pTexture);
}
/**
unuse Texture with slot id
*/
void DXTexture::UnBind(int rtextureid)
{
	ID3D11ShaderResourceView* pnull_srv = nullptr;
	pDeviceContext->PSSetShaderResources(rtextureid, 1, &pnull_srv);
}
