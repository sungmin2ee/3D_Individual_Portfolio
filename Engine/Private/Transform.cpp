#include "Transform.h"

CTransform::CTransform(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CComponent{ pDevice, pContext }
{
}

CTransform::~CTransform()
{

}

_float3 CTransform::Get_Scaled()
{
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK)))
    );
}

HRESULT CTransform::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    auto        pDesc = static_cast<TRANSFORM_DESC*>(pArg);

    m_fSpeedPerSec = pDesc->fSpeedPerSec;
    m_fRotationPerSec = pDesc->fRotationPerSec;

    return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
    _vector         vPosition = Get_State(STATE::POSITION);
    _vector         vLook = Get_State(STATE::LOOK);

    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
    _vector         vPosition = Get_State(STATE::POSITION);
    _vector         vLook = Get_State(STATE::LOOK);

    vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
    _vector         vPosition = Get_State(STATE::POSITION);
    _vector         vRight = Get_State(STATE::RIGHT);

    vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
    _vector         vPosition = Get_State(STATE::POSITION);
    _vector         vRight = Get_State(STATE::RIGHT);

    vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Rotation(_fvector vAxis, _float fAngle)
{
    _float3     vScaled = Get_Scaled();

    _vector     vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
    _vector     vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
    _vector     vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

    _matrix     RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fAngle));

    Set_State(STATE::RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
    Set_State(STATE::UP, XMVector3TransformNormal(vUp, RotationMatrix));
    Set_State(STATE::LOOK, XMVector3TransformNormal(vLook, RotationMatrix));

    /*XMVector4Transform();
    XMVector3TransformCoord();*/

}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
    _float3     vScaled = Get_Scaled();

    _vector     vRight = Get_State(STATE::RIGHT);
    _vector     vUp = Get_State(STATE::UP);
    _vector     vLook = Get_State(STATE::LOOK);

    _matrix     RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(m_fRotationPerSec) * fTimeDelta);

    Set_State(STATE::RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
    Set_State(STATE::UP, XMVector3TransformNormal(vUp, RotationMatrix));
    Set_State(STATE::LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::LookAt(_fvector vAt)
{
    _vector     vLook = vAt - Get_State(STATE::POSITION);
    _vector     vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector     vUp = XMVector3Cross(vLook, vRight);

    _float3     vScaled = Get_Scaled();

    Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
    Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
    Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    Set_State(STATE::RIGHT, Get_State(STATE::RIGHT) * fScaleX);
    Set_State(STATE::UP, Get_State(STATE::UP) * fScaleY);
    Set_State(STATE::LOOK, Get_State(STATE::LOOK) * fScaleZ);
}



void CTransform::Chase(_fvector vGoal, _float fTimeDelta, _float fLimit)
{
    _vector     vPosition = Get_State(STATE::POSITION);
    _vector     vMoveDir = vGoal - vPosition;

    _float      fDistance = XMVectorGetX(XMVector3Length(vMoveDir));


    if (fDistance > fLimit)
        vPosition += XMVector3Normalize(vMoveDir) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE::POSITION, vPosition);
}

shared_ptr<CTransform> CTransform::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = shared_ptr<CTransform>(new CTransform(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTransform");
        return nullptr;
    }

    return pInstance;
}



shared_ptr<CPrototype> CTransform::Clone(void* pArg)
{
    return nullptr;
}
