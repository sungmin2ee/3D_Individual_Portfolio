#include "Shader.h"

Shader::Shader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CComponent(pDevice,pContext), m_pDevice{pDevice}, m_pContext{pContext}
{
}

Shader::~Shader()
{
}
HRESULT Shader::Initialize_Prototype()
{
    HRESULT hr;

    ComPtr<ID3DBlob> pVSBlob = nullptr;
    ComPtr<ID3DBlob> pErrorBlob = nullptr;

    // =========================
    // Vertex Shader
    // =========================
    hr = D3DCompileFromFile(L"../../Shader_VtxNonAnim.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_MAIN", "vs_5_0",
        0, 0, &pVSBlob, &pErrorBlob);

    if (FAILED(hr)) {
        if (pErrorBlob)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        return hr;
    }

    m_pDevice->CreateVertexShader(
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        nullptr,
        &m_pVS);

    // =========================
    // Input Layout
    // =========================
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    //D3D11_INPUT_ELEMENT_DESC ied[] =
    //{
    //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

    //    // [수정] SINT -> UINT (쉐이더의 uint4와 매칭)
    //    { "BLENDINDEX",  0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

    //    { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    //};

    m_pDevice->CreateInputLayout(ied, 4, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pLayout);
    // =========================
    // Pixel Shader
    // =========================
    ComPtr<ID3DBlob> pPSBlob;
    hr = D3DCompileFromFile(L"../../Shader_VtxNonAnim.hlsl",
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_MAIN", "ps_5_0",
        0, 0, &pPSBlob, &pErrorBlob);

    m_pDevice->CreatePixelShader(
        pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(),
        nullptr,
        &m_pPS);

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    hr = m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
    if (FAILED(hr)) return E_FAIL;

    // =========================
    // Matrix 설정 
    // =========================

    // 후면 추려내기 
    D3D11_RASTERIZER_DESC drd = {};
    drd.FillMode = D3D11_FILL_SOLID;      // 면을 채워서 그림
    drd.CullMode = D3D11_CULL_NONE;       // ★ 후면 추려내기 끔 (앞뒤 모두 그림)
    drd.FrontCounterClockwise = FALSE;    // 시계 방향을 앞면으로 간주
    drd.DepthClipEnable = TRUE;           // 깊이 클리핑 활성화

    /* hr = pOutDevice->CreateRasterizerState(&drd, &m_pRasterizerState);
     if (FAILED(hr)) return E_FAIL;*/
     // =========================
     // Constant Buffer
     // =========================
   // 1. MatrixBuffer (b0) 생성
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(MatrixBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_pDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);


	return S_OK;
}

HRESULT Shader::Initialize(void* pArg)
{


	return S_OK;
}

HRESULT Shader::Bind_Matrix(const MatrixBuffer& tBuffer) {
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // 1. 데이터를 GPU 버퍼로 복사
    m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &tBuffer, 0, 0);
    // 2. 파이프라인에 바인딩

    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    m_pContext->IASetInputLayout(m_pLayout.Get());
    m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
    m_pContext->PSSetShader(m_pPS.Get(), 0, 0);

    m_pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
    return S_OK;
}

HRESULT Shader::Bind_Texture(const string& strTag, ComPtr<ID3D11ShaderResourceView> pSRV) {
    if (strTag == "g_DiffuseTexture")
        m_pContext->PSSetShaderResources(0, 1, pSRV.GetAddressOf());
    else if (strTag == "g_NormalTexture")
        m_pContext->PSSetShaderResources(1, 1, pSRV.GetAddressOf());

    return S_OK;
}
unique_ptr<Shader> Shader::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<Shader>(new Shader(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Anim_Shader");
		return nullptr;
	}

	return pInstance;
}
shared_ptr<CPrototype> Shader::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<Shader>(new Shader(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Anim_Shader");
		return nullptr;
	}

	return pInstance;
}