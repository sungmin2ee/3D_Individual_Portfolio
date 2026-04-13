#include "VIBuffer_Model.h"

CVIBuffer_Model::CVIBuffer_Model(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Model::~CVIBuffer_Model()
{

}


HRESULT CVIBuffer_Model::Initialize_Prototype()
{
    HRESULT hr;

    m_iNumVertexBuffers = 1;
    m_iNumVertices = 4;
    m_iVertexStride = sizeof(VERTEX);
    m_iNumIndices = 6;
    m_iIndexStride = 2;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

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

    VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);


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

    uint16_t* pIndices = new uint16_t[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(uint16_t) * m_iNumIndices);


    D3D11_SUBRESOURCE_DATA          IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;

#pragma endregion

    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);


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
    return S_OK;
}

HRESULT CVIBuffer_Model::Initialize(void* pArg)
{

    return S_OK;
}

shared_ptr<CVIBuffer_Model> CVIBuffer_Model::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = shared_ptr<CVIBuffer_Model>(new CVIBuffer_Model(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Model");
        return nullptr;
    }

    return pInstance;
}



shared_ptr<CPrototype> CVIBuffer_Model::Clone(void* pArg)
{
    auto		pInstance = shared_ptr<CVIBuffer_Model>(new CVIBuffer_Model(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CVIBuffer_Model");
        return nullptr;
    }

    return pInstance;
}

