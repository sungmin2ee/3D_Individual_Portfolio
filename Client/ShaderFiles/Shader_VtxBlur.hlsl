
#include "Shader_Defines.hpp"
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

vector g_vCamPosition;


Texture2D<float4> InputTexture : register(t0);
RWTexture2D<float4> OutputTexture : register(u0);
static const float weights[5] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };


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
	//Out.vProjPos = vPosition;

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

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	
	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	
	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	
	vNormal = mul(vNormal, WorldMatrix);
	
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
	
	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}



[numthreads(32, 32, 1)]
void CS_Blur_Horizontal(uint3 DTid : SV_DispatchThreadID)
{
    // 이미지 크기를 가져옵니다.
    uint width, height;
    OutputTexture.GetDimensions(width, height);

    if (DTid.x >= width || DTid.y >= height)
        return;

    float4 blurredColor = float4(0, 0, 0, 0);
    
    // 가로 방향 샘플링
    for (int i = -2; i <= 2; ++i)
    {
        // 경계 처리 (Clamp)
        int2 samplePos = int2(clamp(DTid.x + i, 0, width - 1), DTid.y);
        blurredColor += InputTexture[samplePos] * weights[i + 2];
    }

    OutputTexture[DTid.xy] = blurredColor;
}
[numthreads(32, 32, 1)]
void CS_Blur_Vertical(uint3 DTid : SV_DispatchThreadID)
{
    uint width, height;
    OutputTexture.GetDimensions(width, height);

    if (DTid.x >= width || DTid.y >= height)
        return;

    float4 blurredColor = float4(0, 0, 0, 0);
    
    for (int i = -2; i <= 2; ++i)
    {
        int2 samplePos = int2(DTid.x, clamp(DTid.y + i, 0, height - 1));
        blurredColor += InputTexture[samplePos] * weights[i + 2];
    }

    OutputTexture[DTid.xy] = blurredColor;
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
    pass ComputeBlur_Horizontal
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(compile cs_5_0 CS_Blur_Horizontal());
    }
    
    pass ComputeBlur_Vertical
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(compile cs_5_0 CS_Blur_Vertical());
    }
}

