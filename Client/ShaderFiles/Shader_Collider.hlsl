
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vColor;

//RasterizerState RS_Wireframe
//{
//    FillMode = Wireframe;
//    CullMode = None;
//};




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
    
    float4x4 matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    
    
    Out.vPosition = mul(float4(In.vPos, 1.f), matWVP);
    Out.vColor = In.vColor * g_vColor;
    return Out;
}

float4 PS_MAIN(VS_OUT In) : SV_Target
{
    return In.vColor;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {

        //SetRasterizerState(RS_Wireframe);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();

    }
}

