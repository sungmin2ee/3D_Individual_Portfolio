// 1. 상수 버퍼 정의 (C++에서 넘겨주는 View * Projection 행렬)
cbuffer cbMatrix : register(b0)
{
    matrix g_matVP; // ViewMatrix * ProjectionMatrix
};

// 2. 입력 구조체 정의 (C++의 VOBB와 일치해야 함)
struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

// 3. 출력 구조체 정의 (픽셀 쉐이더로 넘어가는 데이터)
struct VS_OUT
{
    float4 vPosition : SV_POSITION; // 시스템에서 사용하는 위치값
    float4 vColor : COLOR;
};

// Vertex Shader
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    // 로컬/월드 좌표인 vPos를 화면 좌표계(Clip Space)로 변환
    Out.vPosition = mul(float4(In.vPos, 1.f), g_matVP);
    Out.vColor = In.vColor;
    
    return Out;
}

// Pixel Shader
float4 PS_MAIN(VS_OUT In) : SV_Target
{
    // 정점에서 계산된 색상을 그대로 출력 (보간됨)
    return In.vColor;
}