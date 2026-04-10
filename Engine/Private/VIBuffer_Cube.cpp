#include "VIBuffer_Cube.h"
#include "GameInstance.h"

VIBuffer_Cube::VIBuffer_Cube(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{
    pIndices = nullptr;
    pVertices = nullptr;
}

VIBuffer_Cube::VIBuffer_Cube(const VIBuffer_Cube& rhs)
    : CVIBuffer(rhs)
    , m_pVS(rhs.m_pVS)
    , m_pPS(rhs.m_pPS)
    , m_pLayout(rhs.m_pLayout)
    , m_pSamplerState(rhs.m_pSamplerState)
    , m_pConstantBuffer(rhs.m_pConstantBuffer)
{
    // 1. 개수 복사
    m_iNumVertices = rhs.m_iNumVertices;
    m_iNumIndices = rhs.m_iNumIndices;

    // 2. 새로운 메모리 동적 할당 (Deep Copy)
    pVertices = new VOBB[m_iNumVertices];
    memcpy(pVertices, rhs.pVertices, sizeof(VOBB) * m_iNumVertices);

    pIndices = new uint16_t[m_iNumIndices];
    memcpy(pIndices, rhs.pIndices, sizeof(uint16_t) * m_iNumIndices);
}
VIBuffer_Cube::~VIBuffer_Cube()
{
    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);
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

    D3D11_BUFFER_DESC cbDesc{};
    cbDesc.ByteWidth = sizeof(CB_MATRIX);
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);

#pragma endregion


    return S_OK;
}

HRESULT VIBuffer_Cube::Initialize(void* pArg)
{

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
    auto		pInstance = shared_ptr<VIBuffer_Cube>(new VIBuffer_Cube(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : VIBuffer_Cube");
        return nullptr;
    }

    return pInstance;
}

HRESULT VIBuffer_Cube::Render()
{
    CB_MATRIX cb{};
    cb.matVP = XMMatrixTranspose(
        CGameInstance::Get().GetViewXM() *
        CGameInstance::Get().GetProjXM()
    );

    m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());



    D3D11_MAPPED_SUBRESOURCE mapped;
    m_pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, pVertices, sizeof(VOBB) * 8);
    m_pContext->Unmap(m_pVB.Get(), 0);

    // 4. 파이프라인 세팅
    UINT stride = sizeof(VOBB);
    UINT offset = 0;

    m_pContext->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &stride, &offset);
    m_pContext->IASetIndexBuffer(m_pIB.Get(), DXGI_FORMAT_R16_UINT, 0);
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    m_pContext->IASetInputLayout(m_pLayout.Get());
    m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
    m_pContext->PSSetShader(m_pPS.Get(), 0, 0);

    // (선 안 보이면 이거 추가)
   // m_pContext->OMSetDepthStencilState(nullptr, 0);

    // 5. Draw
    m_pContext->DrawIndexed(24, 0, 0);
    return S_OK;
}

