#include "HealthBarFill.h"
#include "Body_Player.h"
#include "Player.h"
#include "Layer.h"

#include "GameInstance.h"

CHealthBarFill::CHealthBarFill(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CHealthBarFill::CHealthBarFill(const CHealthBarFill& Prototype)
	: CUIObject{ Prototype }
{
}

CHealthBarFill::~CHealthBarFill()
{
	int a = 10;
}


HRESULT CHealthBarFill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHealthBarFill::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CHealthBarFill::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CHealthBarFill::Update(_float fTimeDelta)
{


	__super::Update(fTimeDelta);

}

void CHealthBarFill::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CHealthBarFill));

	__super::Late_Update(fTimeDelta);
}

HRESULT CHealthBarFill::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return E_FAIL;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return E_FAIL;
	auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
	if (playerBody == nullptr)
		return E_FAIL;
	_float hp = playerBody->Get_HP();
	if (FAILED(m_pShaderCom->Bind_RawValue("Hp", &hp, sizeof _float)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHealthBarFill::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_HpBar")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_Health_Fill")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CHealthBarFill> CHealthBarFill::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CHealthBarFill>(new CHealthBarFill(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHealthBarFill");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CHealthBarFill::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CHealthBarFill(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHealthBarFill");
		return nullptr;
	}
	
	return pInstance;
}

