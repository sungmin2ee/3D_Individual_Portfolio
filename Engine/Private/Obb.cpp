#include "Obb.h"
#include "GameInstance.h"
#include "Helper.h"
#include "VIBuffer_Cube.h"
Obb::Obb(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CPrototype> bf): CCollider(pDevice, pContext)

{
    m_pBuffer = static_pointer_cast<VIBuffer_Cube>(bf);
}

Obb::~Obb()
{

}
HRESULT Obb::Initialize(void* pArg) {

    return S_OK;
}

HRESULT Obb::Render()
{
    m_pBuffer->Render();
    //__super::Render();
    return S_OK;
}


HRESULT Obb::Initialize_Prototype() {
   
    XMFLOAT3 corners[8];
    myOBB.GetCorners(corners); //

    // 3. Vertex 채우기
    for (int i = 0; i < 8; ++i)
    {
        m_pBuffer->pVertices[i].vPos = corners[i];
        m_pBuffer->pVertices[i].vColor = XMFLOAT4(0, 1, 0, 1);
    }
   // __super::Initialize_Prototype();
   
    return S_OK;
}
//void Obb::Render_Debug()
//{
//    CB_MATRIX cb{};
//
//    // View * Projection (보통 Transpose 필요)
//    cb.matVP = XMMatrixTranspose(CGameInstance::Get().GetViewXM() *CGameInstance::Get().GetProjXM());
//
//    // 1. OBB의 8개 꼭짓점 좌표를 가져옵니다.
//
//    XMFLOAT3 corners[8];
//    myOBB.GetCorners(corners);
//
//    // 2. 8개의 꼭짓점을 선으로 잇기 위해 PrimitiveBatch를 사용합니다.
//    // (아래는 꼭짓점들을 연결하는 인덱스 순서입니다)
//    for (int i = 0; i < 8; ++i) {
//        pVertices[i].vPos = corners[i];
//        pVertices[i].vColor = XMFLOAT4(1.f, 1.f, 0.f, 1.f); // 녹색
//    }
//    m_pContext->IASetInputLayout(m_pLayout.Get());
//    Bind_Resources();
//    m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
//    m_pContext->PSSetShader(m_pPS.Get(), 0, 0);
//
//    m_pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
//    // 1. 데이터를 GPU 버퍼로 복사
//    m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
//    //// 2. 파이프라인에 바인딩
//
//    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
//
//
//
//    D3D11_MAPPED_SUBRESOURCE mapped;
//    m_pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//
//    memcpy(mapped.pData, pVertices, sizeof(VOBB) * m_iNumVertices);
//
//    m_pContext->Unmap(m_pVB.Get(), 0);
//
//    Render();
//
//}

unique_ptr<Obb> Obb::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, shared_ptr<CPrototype> bf)
{
    auto		pInstance = unique_ptr<Obb>(new Obb(pDevice, pContext, bf));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Obb");
        return nullptr;
    }
    return pInstance;
}
shared_ptr<CPrototype> Obb::Clone(void* pArg)
{
    auto pInstance = make_shared<Obb>(*this);

    // 2. 이 부분이 핵심! 
    // 프로토타입이 들고 있던 m_pBuffer를 클론에게도 전달함
    // (이때 RefCount가 올라가며 안전하게 공유됨)

    if (FAILED(pInstance->Initialize(pArg))) return nullptr;

    return pInstance;
}

void Obb::Update_OBB()
{
    XMFLOAT3 corners[8];
    myOBB.GetCorners(corners); //

    // 3. Vertex 채우기
    for (int i = 0; i < 8; ++i)
    {
        m_pBuffer->pVertices[i].vPos = corners[i];
    }

    //_float3 rayPos;
    //_float3 rayDir;

    //CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
    //float dist = 0.f;

    //bool hit = myOBB.Intersects(
    //    XMLoadFloat3(&rayPos),
    //    XMLoadFloat3(&rayDir),
    //    dist
    //);
    //// 2. OBB corners 가져오기
    //XMFLOAT3 corners[8];
    //myOBB.GetCorners(corners); //

    //// 3. Vertex 채우기
    //for (int i = 0; i < 8; ++i)
    //{
    //    m_pBuffer->pVertices[i].vPos = corners[i];
    //    if (hit) {
    //        m_pBuffer->pVertices[i].vColor = XMFLOAT4(1, 0, 0, 1); // 노란색
    //    }
    //    else {
    //        m_pBuffer->pVertices[i].vColor = XMFLOAT4(0, 1, 0, 1); // 노란색
    //    }
    //}
}

bool Obb::Intersects(shared_ptr<CCollider> pTarget)
{
    return false;
}

bool Obb::IntersectsRay()
{
    float dist = 0.f;
    _float3 rayPos;
    _float3 rayDir;

    CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
    bool hit = myOBB.Intersects(
        XMLoadFloat3(&rayPos),
        XMLoadFloat3(&rayDir),
        dist
    );
    XMFLOAT3 corners[8];
    myOBB.GetCorners(corners); //

    // 3. Vertex 채우기
    for (int i = 0; i < 8; ++i)
    {
        m_pBuffer->pVertices[i].vPos = corners[i];
        if (hit) {
            m_pBuffer->pVertices[i].vColor = XMFLOAT4(1, 0, 0, 1); // 노란색
        }
        else {
            m_pBuffer->pVertices[i].vColor = XMFLOAT4(0, 1, 0, 1); // 노란색
        }
    }
    return hit;

}
