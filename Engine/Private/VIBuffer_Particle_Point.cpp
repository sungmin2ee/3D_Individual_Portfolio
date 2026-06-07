#include "VIBuffer_Particle_Point.h"
#include "GameInstance.h"


CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CVIBuffer_Instance { pDevice, pContext }
{
}

CVIBuffer_Particle_Point::~CVIBuffer_Particle_Point()
{
}

HRESULT CVIBuffer_Particle_Point::Initialize_Prototype(void* pArg)
{
    auto        pDesc = static_cast<PARTICLE_INSTANCE_DESC*>(pArg);

    m_iNumVertexBuffers = 2;
    m_iNumVertices = 1;
    m_iVertexStride = sizeof(VTXPOS);
    m_iNumIndices = 0;
    m_iIndexStride = 0;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
    m_iInstanceVertexStride = sizeof(VTXINSTANCE_PARTICLE);
    m_iIndexCountPerInstance = m_iNumIndices;
    m_iNumInstances = pDesc->iNumInstances;

#pragma region VERTEX_BUFFER
 
    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;

    unique_ptr<VTXPOS[]> pVertices = make_unique<VTXPOS[]>(m_iNumVertices);

    pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);

    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = pVertices.get();

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

#pragma endregion


#pragma region INSTANCE_BUFFER
               
    m_InstanceBufferDesc.ByteWidth = m_iNumInstances * m_iInstanceVertexStride;
    m_InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    m_InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_InstanceBufferDesc.StructureByteStride = m_iInstanceVertexStride;
    m_InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_InstanceBufferDesc.MiscFlags = 0;

    // m_pInstanceData = make_shared<VTXINSTANCE_PARTICLE[]>(m_iNumInstances);
    m_pInstanceData = shared_ptr<VTXINSTANCE_PARTICLE[]>(new VTXINSTANCE_PARTICLE[m_iNumInstances]);
    m_pSpeeds = shared_ptr<_float[]>(new _float[m_iNumInstances]);
    m_isLoop = pDesc->isLoop;
    m_vPivot = pDesc->vPivot;

    for (size_t i = 0; i < m_iNumInstances; i++)
    {
        _float      fScale = CGameInstance::Get().Random(pDesc->vSize.x, pDesc->vSize.y);
        m_pSpeeds[i] = CGameInstance::Get().Random(pDesc->vSpeed.x, pDesc->vSpeed.y);

        m_pInstanceData[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
        m_pInstanceData[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
        m_pInstanceData[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
        m_pInstanceData[i].vTranslation = _float4(
            CGameInstance::Get().Random((pDesc->vCenter.x - pDesc->vRange.x) * 0.5f, (pDesc->vCenter.x + pDesc->vRange.x) * 0.5f),
            CGameInstance::Get().Random((pDesc->vCenter.y - pDesc->vRange.y) * 0.5f, (pDesc->vCenter.y + pDesc->vRange.y) * 0.5f),
            CGameInstance::Get().Random((pDesc->vCenter.z - pDesc->vRange.z) * 0.5f, (pDesc->vCenter.z + pDesc->vRange.z) * 0.5f),
            1.f);

        m_pInstanceData[i].vLifeTime = _float2(
            CGameInstance::Get().Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y),
            0.f);

    }       


#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Initialize(void* pArg)
{

    D3D11_SUBRESOURCE_DATA          InstanceInitialData{};
    InstanceInitialData.pSysMem = m_pInstanceData.get();

    if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &InstanceInitialData, &m_pVBInstance)))
        return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Bind_Resources()
{
    ID3D11Buffer* pVertexBuffers[] = {
         m_pVB.Get(),
         m_pVBInstance.Get(),
    };

    uint32_t       iVertexStrides[] = {
        m_iVertexStride,
        m_iInstanceVertexStride
    };

    uint32_t        iOffSets[] = {
        0,
        0,
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffSets);
  
    m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);

    return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Render()
{
    m_pContext->DrawInstanced(1, m_iNumInstances, 0, 0);

    return S_OK;
}
void CVIBuffer_Particle_Point::Set_Center(_float3 vNewCenter)
{
    for (uint32_t i = 0; i < m_iNumInstances; i++)
    {
        m_pInstanceData[i].vTranslation = _float4(
            vNewCenter.x,
            vNewCenter.y,
            vNewCenter.z,
            1.f
        );
    }
    m_vPivot = vNewCenter;
}
void CVIBuffer_Particle_Point::Drop(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE            MappedSubResource{};

    if (FAILED(m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource)))
        return;

    auto        pVertices = static_cast<VTXINSTANCE_PARTICLE*>(MappedSubResource.pData);
    
    for (uint32_t i = 0; i < m_iNumInstances; i++)
    {
        pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;
        pVertices[i].vLifeTime.y += fTimeDelta;

        if (true == m_isLoop && 
            pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            pVertices[i].vTranslation.y = m_pInstanceData[i].vTranslation.y;
            pVertices[i].vLifeTime.y = 0.f;
        }
    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);
}

void CVIBuffer_Particle_Point::Rise(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE            MappedSubResource{};

    if (FAILED(m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource)))
        return;

    auto        pVertices = static_cast<VTXINSTANCE_PARTICLE*>(MappedSubResource.pData);

    for (uint32_t i = 0; i < m_iNumInstances; i++)
    {
        pVertices[i].vTranslation.y += m_pSpeeds[i] * fTimeDelta;
        pVertices[i].vLifeTime.y += fTimeDelta;

        if (true == m_isLoop &&
            pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            pVertices[i].vTranslation.y = m_pInstanceData[i].vTranslation.y;
            pVertices[i].vLifeTime.y = 0.f;
        }
    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);
}

void CVIBuffer_Particle_Point::Spread(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE            MappedSubResource{};

    if (FAILED(m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource)))
        return;

    auto        pVertices = static_cast<VTXINSTANCE_PARTICLE*>(MappedSubResource.pData);

    for (uint32_t i = 0; i < m_iNumInstances; i++)
    {
        // pVertices[i].vTranslation.y += m_pSpeeds[i] * fTimeDelta;
        pVertices[i].vLifeTime.y += fTimeDelta;

        _vector     vMoveDir = XMVectorSetW(XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_vPivot), 0.f);

        XMStoreFloat4(&pVertices[i].vTranslation,
            XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vMoveDir) * m_pSpeeds[i] * fTimeDelta);




        if (true == m_isLoop &&
            pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            pVertices[i].vTranslation = m_pInstanceData[i].vTranslation;
            pVertices[i].vLifeTime.y = 0.f;
        }
    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);
}

void CVIBuffer_Particle_Point::Blood(_float fTimeDelta)
{
    D3D11_MAPPED_SUBRESOURCE MappedSubResource{};
    if (FAILED(m_pContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource)))
        return;

    auto pVertices = static_cast<VTXINSTANCE_PARTICLE*>(MappedSubResource.pData);

    if (m_Reset) {
        for (uint32_t i = 0; i < m_iNumInstances; i++) {
            pVertices[i].vTranslation = m_pInstanceData[i].vTranslation;
            pVertices[i].vLifeTime.y = 0.f;
        }
        m_Reset = false;
    }

    // 상수 정의 (액체의 무게감 조절용)
    const _float fGravity = 9.81f;       // 중력 가속도
    const _float fAirResistance = 0.8f;  // 공기 저항 (높을수록 피가 가다가 멈칫하며 뚝뚝 떨어짐)

    for (uint32_t i = 0; i < m_iNumInstances; i++)
    {
        // 1. 수명 진행
        pVertices[i].vLifeTime.y += fTimeDelta;
        _float fCurrentAge = pVertices[i].vLifeTime.y; // 흘러간 시간
        _vector vRandomDir = XMVectorSet(
            CGameInstance::Get().Random(-1.f, 1.f),  // 좌우로 퍼짐
            CGameInstance::Get().Random(0.9f, 1.5f),   // 위로 솟구침 
            CGameInstance::Get().Random(-0.8f, 0.8f),  // 앞뒤로 퍼짐
            0.f
        );

        // 2. 처음에 설정된 고유 발사 방향(vLook) 로드
        _float fSpeed = m_pSpeeds[i];

        // 3. 물리 연산 (공기 저항에 의해 갈수록 발사 속도가 힘을 잃음)
        _float fReducedSpeed = fSpeed * exp(-fAirResistance * fCurrentAge);
        _vector vVelocity = vRandomDir * fReducedSpeed;

        // 4. 중력 적용 (시간이 흐를수록 Y축 하강 속도가 제곱비례로 증가)
        // 포물선 운동 공식: 의도한 초속도 방향 세팅 후 Y축에만 중력 누적
        vVelocity = XMVectorSetY(vVelocity, XMVectorGetY(vVelocity) - (fGravity * fCurrentAge * 0.5f));

        // 5. 최종 위치 적용
        _vector vCurrentPos = XMLoadFloat4(&pVertices[i].vTranslation);
        XMStoreFloat4(&pVertices[i].vTranslation, vCurrentPos + vVelocity * fTimeDelta);

        // 6. 루프 및 소멸 조건
        if (true == m_isLoop && pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
        {
            pVertices[i].vTranslation = m_pInstanceData[i].vTranslation;
            pVertices[i].vLifeTime.y = 0.f;
        }
    }

    m_pContext->Unmap(m_pVBInstance.Get(), 0);

}


unique_ptr<CVIBuffer_Particle_Point> CVIBuffer_Particle_Point::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, void* pArg)
{
    auto		pInstance = unique_ptr<CVIBuffer_Particle_Point>(new CVIBuffer_Particle_Point(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype(pArg)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Particle_Point");
        return nullptr;
    }

    return pInstance;
}



shared_ptr<CPrototype> CVIBuffer_Particle_Point::Clone(void* pArg)
{
    auto		pInstance = shared_ptr<CVIBuffer_Particle_Point>(new CVIBuffer_Particle_Point(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Point");
        return nullptr;
    }

    return pInstance;
}

