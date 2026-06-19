#include "Layer.h"
#include "Nuclear.h"

#include "GameInstance.h"

CNuclear::CNuclear(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}

CNuclear::CNuclear(const CNuclear& Prototype)
	: CGameObject{ Prototype }
{
}

CNuclear::~CNuclear()
{

}


HRESULT CNuclear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNuclear::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_Scale(3.8f,3.f,1.f);
	m_pTransformCom->Set_State(STATE::POSITION,XMVectorSet(-2.5f,1.2f,3.f,1.f));

	return S_OK;
}

void CNuclear::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CNuclear::Update(_float fTimeDelta)
{


	if (!m_bStart)
		return;
	_float4 myPos;
	XMStoreFloat4(&myPos, m_pTransformCom->Get_State(STATE::POSITION));
	m_pTransformCom->Set_Scale(m_pTransformCom->Get_Scaled().x + fTimeDelta, m_pTransformCom->Get_Scaled().y + fTimeDelta, m_pTransformCom->Get_Scaled().z);

	myPos.z -= (fTimeDelta * 0.3f);
	m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&myPos));
	m_fEffectTime +=fTimeDelta;
	if (m_fEffectTime > 0.15f) {
		if (m_iCurIndex < 11) {
			m_iCurIndex++;
		}
		m_fEffectTime = 0.f;
	}


	if (m_iCurIndex == 8) {
		// 하얀색 화면으로 마무리
		auto NuclearLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Nuclear");
		if (NuclearLayer == nullptr) return;
		auto objects = NuclearLayer->GetObjects();
		for (auto& obj : objects) {
			if (obj->Get_Tag() == L"WhiteRect") {
				static_pointer_cast<CUIObject>(obj)->Set_Render(true);
				break;
			}
		}

	}
	__super::Update(fTimeDelta);

	

}

void CNuclear::Late_Update(_float fTimeDelta)
{

	if (!m_bStart)
		return;
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CNuclear));

	__super::Late_Update(fTimeDelta);
}

HRESULT CNuclear::Render()
{
	if (!m_bStart)
		return E_FAIL;

	const _float4x4* view;
	const _float4x4* proj;
	view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
	proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);


	_float4x4 mat = m_pTransformCom->GetWorld();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &mat)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_NumRow", &m_iRow, sizeof(uint32_t))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_NumCol", &m_iCol, sizeof(uint32_t))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentFrame", &m_iCurIndex, sizeof(uint32_t))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CNuclear::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_Nuclear")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}


unique_ptr<CNuclear> CNuclear::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CNuclear>(new CNuclear(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNuclear");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CNuclear::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CNuclear(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNuclear");
		return nullptr;
	}
	return pInstance;
}

