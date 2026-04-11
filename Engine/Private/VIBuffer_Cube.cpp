#include "VIBuffer_Cube.h"
#include "GameInstance.h"

VIBuffer_Cube::VIBuffer_Cube(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{
    pIndices = nullptr;
    pVertices = nullptr;
}

VIBuffer_Cube::~VIBuffer_Cube()
{

}


HRESULT VIBuffer_Cube::Initialize_Prototype()
{
    m_iNumVertexBuffers = 1;
    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VOBB);
    m_iNumIndices = 24;
    m_iIndexStride = 2;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

    pVertices = new VOBB[m_iNumVertices];

    pVertices[0].vPos = _float3(-0.5f, 0.5f, -0.5f);
    pVertices[1].vPos = _float3(0.5f, 0.5f, -0.5f);
    pVertices[2].vPos = _float3(0.5f, -0.5f, -0.5f);
    pVertices[3].vPos = _float3(-0.5f, -0.5f, -0.5f);
    pVertices[4].vPos = _float3(-0.5f, 0.5f, 0.5f);
    pVertices[5].vPos = _float3(0.5f, 0.5f, 0.5f);
    pVertices[6].vPos = _float3(0.5f, -0.5f, 0.5f);
    pVertices[7].vPos = _float3(-0.5f, -0.5f, 0.5f);

    for (int i = 0; i < 8; ++i)
        pVertices[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
#pragma region VERTEX_BUFFER
    /*
     UINT ByteWidth;
     D3D11_USAGE Usage;
     UINT BindFlags;
     UINT CPUAccessFlags;
     UINT MiscFlags;
     UINT StructureByteStride;
    */
    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;




    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

#pragma endregion


#pragma region INDEX_BUFFER
    D3D11_BUFFER_DESC           IndexBufferDesc{};
    IndexBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.StructureByteStride = m_iIndexStride;
    IndexBufferDesc.CPUAccessFlags = 0;
    IndexBufferDesc.MiscFlags = 0;

    pIndices = new uint16_t[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(uint16_t) * m_iNumIndices);


    pIndices[0] = 0;
    pIndices[1] = 1;

    pIndices[2] = 1;
    pIndices[3] = 2;

    pIndices[4] = 2;
    pIndices[5] = 3;

    pIndices[6] = 3;
    pIndices[7] = 0;

    pIndices[8] = 1;
    pIndices[9] = 5;

    pIndices[10] = 2;
    pIndices[11] = 6;

    pIndices[12] = 3;
    pIndices[13] = 7;

    pIndices[14] = 0;
    pIndices[15] = 4;

    pIndices[16] = 4;
    pIndices[17] = 5;

    pIndices[18] = 5;
    pIndices[19] = 6;

    pIndices[20] = 6;
    pIndices[21] = 7;

    pIndices[22] = 7;
    pIndices[23] = 4;



    D3D11_SUBRESOURCE_DATA          IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;


    HRESULT hr;

    ComPtr<ID3DBlob> pVSBlob = nullptr;
    ComPtr<ID3DBlob> pErrorBlob = nullptr;

    // =========================
    // Vertex Shader
    // =========================
    hr = D3DCompileFromFile(L"../../Collider.hlsl",
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
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_pDevice->CreateInputLayout(ied, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pLayout);;
    // =========================
    // Pixel Shader
    // =========================
    ComPtr<ID3DBlob> pPSBlob;
    hr = D3DCompileFromFile(L"../../Collider.hlsl",
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

   

    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);

#pragma endregion


    return S_OK;
}

HRESULT VIBuffer_Cube::Initialize(void* pArg)
{
    m_pConstantBuffer = nullptr;
    D3D11_BUFFER_DESC cbDesc{};
    cbDesc.ByteWidth = sizeof(CB_MATRIX);
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);
    return S_OK;
}



unique_ptr<VIBuffer_Cube> VIBuffer_Cube::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = unique_ptr<VIBuffer_Cube>(new VIBuffer_Cube(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : VIBuffer_Cube");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CPrototype> VIBuffer_Cube::Clone(void* pArg)
{
    // 1. 복사 생성자를 호출하여 포인터들을 일단 똑같이 복사함
    auto pInstance = shared_ptr<VIBuffer_Cube>(new VIBuffer_Cube(*this));

    // 2. Initialize에서 나만의 "상수 버퍼"를 새로 생성함
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : VIBuffer_Cube");
        return nullptr;
    }

    return pInstance;
}

HRESULT VIBuffer_Cube::Render()
{

    if (FAILED(Bind_Resources()))
        return E_FAIL;

    m_pContext->IASetInputLayout(m_pLayout.Get());

    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
    m_pContext->PSSetShader(m_pPS.Get(), 0, 0);

    return __super::Render();
    
}

HRESULT VIBuffer_Cube::Bind_ConstantBuffer(const CB_MATRIX& cbData)
{
    // 1. 데이터 업데이트
    m_pContext->UpdateSubresource(
        m_pConstantBuffer.Get(),
        0,
        nullptr,
        &cbData,
        0,
        0
    );

    // 2. VS에 바인딩
    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    return S_OK;
}

void VIBuffer_Cube::UpdateConstantBuffer(const CB_MATRIX& data)
{
    m_pContext->UpdateSubresource(
        m_pConstantBuffer.Get(),
        0,
        nullptr,
        &data,
        0,
        0
    );
}