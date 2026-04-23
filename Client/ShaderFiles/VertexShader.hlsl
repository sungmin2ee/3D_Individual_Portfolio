
#include "Shader_Defines.hpp"

// 1. 행렬 상수 버퍼 (Slot 0)
cbuffer MatrixBuffer : register(b0)
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

// 2. 본 행렬 상수 버퍼 (Slot 1)
// struct가 아닌 cbuffer로 감싸야 register 바인딩이 가능합니다.
cbuffer BoneBuffer : register(b1)
{
    matrix g_BoneMatrices[256];
};

Texture2D g_DiffuseTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    // 1. [임시 주석] 본 변환을 하지 않을 때는 이 계산이 필요 없습니다.
    /*
    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x
                      + g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y
                      + g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z
                      + g_BoneMatrices[In.vBlendIndex.w] * fWeightW;
    */

    // 2. [수정] 본 변환(BoneMatrix)을 곱하지 않고 로컬 좌표를 그대로 사용합니다.
    vector vLocalPos = float4(In.vPosition, 1.f);
    
    // 3. 월드/뷰/투영 변환 (Row-Major 연산)
    matrix matWVP = mul(mul(g_WorldMatrix, g_ViewMatrix), g_ProjMatrix);
    Out.vPosition = mul(vLocalPos, matWVP);

    // 4. 노멀 변환도 본 변환 제외
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix).xyz);

    Out.vTexUV = In.vTexUV;
    Out.vWorldPos = mul(vLocalPos, g_WorldMatrix);
    Out.vProjPos = Out.vPosition;

    return Out;
}

// ... PS_MAIN 및 technique11 부분은 동일 ...
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;	

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}



technique11 DefaultTechinque
{
	pass Default
	{
		SetBlendState(BS_None, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
	
}

