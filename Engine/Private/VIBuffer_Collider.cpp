#include "VIBuffer_Collider.h"
#include "GameInstance.h"

VIBuffer_Collider::VIBuffer_Collider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{

}

VIBuffer_Collider::~VIBuffer_Collider()
{

}


HRESULT VIBuffer_Collider::Initialize_Prototype()
{
    m_iNumVertexBuffers = 1;
    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VCOLLIDER);
    m_iNumIndices = 24;
    m_iIndexStride = 2;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

    VCOLLIDER* pVertices = new VCOLLIDER[m_iNumVertices];

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

    uint16_t* pIndices = new uint16_t[m_iNumIndices];
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


   
    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);

#pragma endregion


    return S_OK;
}

HRESULT VIBuffer_Collider::Initialize(void* pArg)
{
 
    return S_OK;
}



unique_ptr<VIBuffer_Collider> VIBuffer_Collider::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = unique_ptr<VIBuffer_Collider>(new VIBuffer_Collider(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : VIBuffer_Collider");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CPrototype> VIBuffer_Collider::Clone(void* pArg)
{
    // 1. 복사 생성자를 호출하여 포인터들을 일단 똑같이 복사함
    auto pInstance = shared_ptr<VIBuffer_Collider>(new VIBuffer_Collider(*this));

    // 2. Initialize에서 나만의 "상수 버퍼"를 새로 생성함
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : VIBuffer_Collider");
        return nullptr;
    }

    return pInstance;
}

