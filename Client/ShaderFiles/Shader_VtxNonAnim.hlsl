
#include "Shader_Defines.hpp"
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_FogColor = float4(0.5f, 0.5f, 0.5f, 1.0f); // 안개 색상 (회색)
float g_FogStart = 10.0f; // 안개가 시작되는 거리
float g_FogEnd = 20.0f; // 완전히 안개에 가려지는 거리

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

sampler LinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal: BINORMAL;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);


    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix).xyz);
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_SOCKET(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matVP;

	
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, matVP);

	Out.vPosition = vPosition;
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix).xyz;
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal: BINORMAL;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In) :SV_Target
{
	PS_OUT		Out = (PS_OUT)0;	

    //vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    //vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f) +
    //    (g_vLightAmbient * g_vMtrlAmbient);
    //
    //vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    //vector vLook = In.vWorldPos - g_vCamPosition;
    //
    //float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
    //
    //Out.vDiffuse = g_vLightDiffuse * vMtrlDiffuse * saturate(vShade) +
    //    (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
	

	
	
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
    if (Out.vDiffuse.a < 0.1f)
        discard;
	
    float fDist = distance(In.vWorldPos.xyz, g_vCamPosition.xyz);
	// 선형 안개 계수 계산 (0: 안개 없음, 1: 완전 안개)
    // float fFogFactor = saturate((fDist - g_FogStart) / (g_FogEnd - g_FogStart));
	
	
	
    float fDensity = 0.405f; // 이 값을 조절해서 농도를 맞추세요
    float fFogFactor = 1.0f - saturate(1.0f / exp(fDist * fDensity));

    // 최종 디퓨즈 색상에 안개 입히기
    // lerp(원래색, 안개색, 안개비율)
    Out.vDiffuse.rgb = lerp(Out.vDiffuse.rgb, g_FogColor.rgb, fFogFactor);

    // 3. 노멀 및 뎁스 정보 저장 (기존과 동일)
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	
	
	//vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	//
	//float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	//
	//float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	//
	//vNormal = mul(vNormal, WorldMatrix);
	//
	//Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	

    //Out.vDiffuse = float4(fFogFactor, fFogFactor, fFogFactor, 1.0f);
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

	pass Socket
	{
		SetBlendState(BS_None, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_SOCKET();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	
}

