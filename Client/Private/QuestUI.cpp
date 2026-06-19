#include "QuestUI.h"
#include "Layer.h"
#include "Overlay.h"

#include "GameInstance.h"

CQuestUI::CQuestUI(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CQuestUI::CQuestUI(const CQuestUI& Prototype)
	: CUIObject{ Prototype }
{
}

CQuestUI::~CQuestUI()
{
}


HRESULT CQuestUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuestUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CQuestUI::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CQuestUI::Update(_float fTimeDelta)
{
	
	//m_bRender = true;

	if (m_bRender == true) {
		auto overlaylayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Overlay"));
		auto overlay = overlaylayer->GetObjectFirst();
		static_pointer_cast<COverlay>(overlay)->Set_ForQuest(0);
		static_pointer_cast<COverlay>(overlay)->Set_Render(true);
	}

	
	__super::Update(fTimeDelta);
	if (CGameInstance::Get().Key_Down(DIK_SPACE)) {
		auto overlaylayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Overlay"));
		auto overlay = overlaylayer->GetObjectFirst();
		static_pointer_cast<COverlay>(overlay)->Set_ForQuest(1);
		static_pointer_cast<COverlay>(overlay)->Set_Render(false);
		m_bDead = true;
	}
}

void CQuestUI::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CQuestUI));

	__super::Late_Update(fTimeDelta);
}

HRESULT CQuestUI::Render()
{
	if (!m_bRender)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	CGameInstance::Get().RenderText(1, L"이 은신처는 곧 파괴됩니다.", (m_fX - m_fSizeX * 0.28f), (m_fY - m_fSizeY * 0.65f),XMVectorSet((217/255.f), (180 / 255.f), (113 / 255.f), 1.f), 1.f);
	CGameInstance::Get().RenderText(0, L"나무와 못들을 모아 은신처를 강화 하세요.", (m_fX - m_fSizeX * 0.25f), (m_fY + m_fSizeY * 0.35f), DirectX::Colors::White, 0.6f);

	return S_OK;
}

HRESULT CQuestUI::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), TEXT("Prototype_Component_Texture_QuestUI")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

unique_ptr<CQuestUI> CQuestUI::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CQuestUI>(new CQuestUI(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CQuestUI");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CQuestUI::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CQuestUI(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CQuestUI");
		return nullptr;
	}

	return pInstance;
}

