#include "ECamera.h"
#include "GameInstance.h"

ECamera::ECamera(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CGameObject{ pDevice, pContext }
{
}

ECamera::ECamera(const ECamera& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT ECamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT ECamera::Initialize(void* pArg)
{
    auto        pDesc = static_cast<CAMERA_DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&pDesc->vEye));
    m_pTransformCom->LookAt(XMLoadFloat4(&pDesc->vAt));

    m_fFovy = pDesc->fFovy;
    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;

    Update_PipeLine();

    return S_OK;
}

void ECamera::Priority_Update(_float fTimeDelta)
{
}

void ECamera::Update(_float fTimeDelta)
{
}

void ECamera::Late_Update(_float fTimeDelta)
{
}

HRESULT ECamera::Render()
{
    return S_OK;
}

void ECamera::Update_PipeLine()
{
    CGameInstance::Get().Set_Transform(D3DTS::VIEW,
        XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr())));

    _float2     vViewportSize = CGameInstance::Get().Get_ViewportSize();

    CGameInstance::Get().Set_Transform(D3DTS::PROJ,
        XMMatrixPerspectiveFovLH(m_fFovy, vViewportSize.x / vViewportSize.y, m_fNear, m_fFar));
}
