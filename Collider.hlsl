cbuffer cbMatrix : register(b0)
{
    matrix g_matWVP;
    float4 g_vColor;
};

struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    Out.vPosition = mul(float4(In.vPos, 1.f), g_matWVP);
    Out.vColor = In.vColor * g_vColor;
    return Out;
}

float4 PS_MAIN(VS_OUT In) : SV_Target
{
    return In.vColor;
}