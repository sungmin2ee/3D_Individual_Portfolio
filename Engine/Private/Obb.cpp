#include "Obb.h"

Obb::Obb(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext):CVIBuffer(pDevice, pContext)
{
}

Obb::~Obb()
{
    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);
}
HRESULT Obb::Initialize(void* pArg) {

    return S_OK;
}


HRESULT Obb::Initialize_Prototype() {
    m_iNumVertexBuffers = 1;
    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VOBB);
    m_iNumIndices = 24;
    m_iIndexStride = 2;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

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
    VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    VertexBufferDesc.MiscFlags = 0;

    pVertices = new VOBB[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VOBB) * m_iNumVertices);




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
    hr = D3DCompileFromFile(L"../../Obb.hlsl",
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
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_pDevice->CreateInputLayout(ied, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pLayout);;
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


#pragma endregion

   

    return S_OK;
}
void Obb::Render_Debug()
{
    // 1. OBB의 8개 꼭짓점 좌표를 가져옵니다.
    XMFLOAT3 corners[8];
    myOBB.GetCorners(corners);

    // 2. 8개의 꼭짓점을 선으로 잇기 위해 PrimitiveBatch를 사용합니다.
    // (아래는 꼭짓점들을 연결하는 인덱스 순서입니다)
    for (int i = 0; i < 8; ++i) {
        pVertices[i].vPos = corners[i];
        pVertices[i].vColor = XMFLOAT4(1.f, 1.f, 0.f, 1.f); // 녹색
    }
    D3D11_MAPPED_SUBRESOURCE mapped;
    m_pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

    memcpy(mapped.pData, pVertices, sizeof(VOBB) * m_iNumVertices);

    m_pContext->Unmap(m_pVB.Get(), 0);

    Bind_Resources();
    Render();

}
shared_ptr<Obb> Obb::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = shared_ptr<Obb>(new Obb(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Obb");
        return nullptr;
    }

    return pInstance;
}
shared_ptr<CPrototype> Obb::Clone(void* pArg)
{
    return nullptr;
}
