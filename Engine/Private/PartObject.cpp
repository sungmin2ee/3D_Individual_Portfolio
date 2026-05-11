#include "PartObject.h"
#include "GameInstance.h"

CPartObject::CPartObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CGameObject { pDevice, pContext }
{
}

CPartObject::CPartObject(const CPartObject& Prototype)
    : CGameObject{ Prototype }
{
}

CPartObject::~CPartObject()
{
}

HRESULT CPartObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
    auto        pDesc = static_cast<PARTOBJECT_DESC*>(pArg);

    m_pParentMatrix = pDesc->pParentMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPartObject::Priority_Update(_float fTimeDelta)
{
}

void CPartObject::Update(_float fTimeDelta)
{
}

void CPartObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CPartObject::Render()
{
    return S_OK;
}

void CPartObject::Make_CombinedWorldMatrix(_fmatrix ChildMatrix)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        ChildMatrix * XMLoadFloat4x4(m_pParentMatrix));
}

HRESULT CPartObject::Bind_WorldMatrix(shared_ptr<class CShader>  pShader, const _char* pConstantName)
{
    return pShader->Bind_Matrix(pConstantName, &m_CombinedWorldMatrix);    
}
