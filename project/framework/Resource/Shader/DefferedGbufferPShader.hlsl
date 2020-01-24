////////////////////////////////////////////////////////////////////////////////
// Filename: Gbuffer Pixel Shader
////////////////////////////////////////////////////////////////////////////////
#include "ShaderUtil.hlsl"
cbuffer ObjectInfo : register(b1)
{
	float3 Diffuse;
	float Shininess;
	float3 Specular;
	int MaterialID;
	uint4 TextureIO;

};
Texture2D DiffuseTexture: register(t7);
Texture2D SpecularTexture: register(t8);
Texture2D ShininessTexture: register(t9);
Texture2D NormalTexture: register(t10);
cbuffer LightList : register(b3)
{
	LightInfo light;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldpos : WORLDPOS;
	float3 eyespos : EYESPOS;
};
struct PixelOutputType
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 diffuse : SV_Target2;
	float4 specular : SV_Target3;
	float4 object1 : SV_Target4;
	float4 object2 : SV_Target5;
};

PixelOutputType main(PixelInputType input) : SV_TARGET
{

	PixelOutputType output;
	output.worldPosition = input.worldpos;
	float4 normal = float4(input.normal, 0);
	if (TextureIO.w == 1)
	{
		normal = NormalTexture.Sample(samLinear, input.tex);
	}
	output.normal = normal;

	float4 diffuse= float4(Diffuse, 1);
	if (TextureIO.x == 1)
	{
		diffuse = DiffuseTexture.Sample(samLinear, input.tex);
		if (SKYDOME == MaterialID)
		{
			float3 V = input.eyespos.xyz - input.worldpos.xyz;
			float3 N = normalize(V);
			float2 UV = float2(1.0f / 2.0f - atan2(N.y , N.x) / (2.0f * pi),1- acos(N.z) / pi);
			diffuse = DiffuseTexture.Sample(samLinear, UV);
		}
	}
	output.diffuse = diffuse;
	if (diffuse.w < 1 && (MaterialID == 2|| MaterialID == 3))
	{
		discard;
	}
	float4 specular= float4(Specular.xyz, Shininess);
	if (TextureIO.y == 1)
	{
		specular.xyz = SpecularTexture.Sample(samLinear, input.tex).xyz;
	}
	if (TextureIO.z == 1)
	{
		specular.w = ShininessTexture.Sample(samLinear, input.tex).x;
	}

	output.specular = specular;
	output. object1 = float4(MaterialID, 0, 0, 0);
	output. object2 = float4(input.position.w, input.position.w, input.position.w, 1);
	

	return output;
}
