#include "SearchBox.h"
#include "SearchItemFrame.h"
#include "SearchItemIcon.h"
#include "Layer.h"
#include "EquipBorder.h"
#include "ItemIcon.h"

#include "GameInstance.h"

CSearchBox::CSearchBox(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CSearchBox::CSearchBox(const CSearchBox& Prototype)
	: CUIObject{ Prototype }
{
}

CSearchBox::~CSearchBox()
{

}


HRESULT CSearchBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSearchBox::Initialize(void* pArg)
{
	auto		pDesc = static_cast<SEARCH_DESC*>(pArg);


	pDesc->pGameObjectTag = TEXT("Search_Frame");
	pDesc->fSpeedPerSec = 0.f;
	pDesc->fRotationPerSec = 0.f;

	pDesc->fSizeX = g_iWinSizeX * 0.25f;
	pDesc->fSizeY = g_iWinSizeY * 0.6f;

	pDesc->fX = g_iWinSizeX * 0.15f;
	pDesc->fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CSearchBox::Priority_Update(_float fTimeDelta)
{
	//CItemIcon::ITEMICON_DESC pDesc;
	//pDesc.bInitialRender = this->m_bRender;
	//pDesc.fSizeX = g_iWinSizeX * 0.4f * 0.15f;
	//pDesc.fSizeY = g_iWinSizeY * 0.7f * 0.15f;
	//pDesc.fX = framePos[itemCount].first;
	//pDesc.fY = framePos[itemCount].second;
	//pDesc.pGameObjectTag = itemTag.first;
	//if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(CGameInstance::Get().GetCurLevelIndex(),
	//	L"Prototype_ItemIcon", CGameInstance::Get().GetCurLevelIndex(), L"UI_ICons", &pDesc)))
	//	return;
	if (m_bClear) {
		auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons");
		if (layer) {
			auto icons = layer->GetObjects();
			for (auto icon : icons) {
				icon->Set_Dead();
			}
		}
		m_bClear = false;
	}
	if (m_bRefresh) {
		itemCount = 0;
		CGameInstance::Get().MakeRandomItem();

		auto items = CGameInstance::Get().Get_FoundItem();

		//auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons");
		//if (layer) {
		//	auto icons = layer->GetObjects();
		//	for (auto icon : icons) {
		//		icon->Set_Dead();
		//	}
		//}

		vector<CItem_Manager::ITEMINFO_DESC> infos = CGameInstance::Get().Get_ItemInfo();

		for (uint32_t i = 0; i < items.size(); ++i) {
			CItemIcon::ITEMICON_DESC pDesc;
			pDesc.bInitialRender = true;
			pDesc.fSizeX = g_iWinSizeX * 0.3f * 0.15f;
			pDesc.fSizeY = g_iWinSizeY * 0.6f * 0.15f;
			pDesc.fX = framePos[itemCount].first;
			pDesc.fY = framePos[itemCount].second;

			pDesc.pGameObjectTag = items[i];
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(CGameInstance::Get().GetCurLevelIndex(),
				L"Prototype_SearchItemIcon", CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons", &pDesc)))
				return ;
			itemCount++;
		}

		m_bRefresh = false;
	}

	
	

	
	__super::Priority_Update(fTimeDelta);
}

void CSearchBox::Update(_float fTimeDelta)
{
	
	__super::Update(fTimeDelta);
	//if (CGameInstance::Get().Key_Down(DIK_E)) {
	//	m_bRefresh = !m_bRefresh;
	//	m_bRender = !m_bRender;
	//}
	if (CGameInstance::Get().Key_Down(DIK_SPACE)) {
		auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons");

		auto items = CGameInstance::Get().Get_FoundItem();

		auto j = items.size();
		for (auto item : items) {
			CGameInstance::Get().Add_Item(item);

		}

		
		if (m_bRender) {

			auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons");
			if (layer == nullptr)
				return;
			auto items = layer->GetObjects();
			for (auto& item : items) {
				static_pointer_cast<CItemIcon>(item)->Set_Render(true);
			}
		}
		else {
			auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"UI_SearchICons");
			if (layer == nullptr)
				return;
			auto items = layer->GetObjects();
			for (auto& item : items) {
				static_pointer_cast<CItemIcon>(item)->Set_Render(false);
			}
		}
		Clear();
		//if (layer) {
		//	auto icons = layer->GetObjects();
		//	for (auto item : items) {
		//		CGameInstance::Get().Add_Item(item);
		//		
		//	}
		//}
	}

}

void CSearchBox::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CSearchBox));

	__super::Late_Update(fTimeDelta);
}

HRESULT CSearchBox::Render()
{
	if (!m_bRender)
		return S_OK;
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

	

	CGameInstance::Get().RenderText(1,L"Å½»ö", (m_fX - m_fSizeX * 0.5f + 30.f), (m_fY - m_fSizeY* 0.5f - 40.f), DirectX::Colors::Gold, 0.9f);

	return S_OK;
}

HRESULT CSearchBox::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_search_border")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CSearchBox::Ready_ItemFrames(LEVEL nextLevel)
{



	_float fCellSizeX = g_iWinSizeX * 0.3f * 0.19f;
	_float fCellSizeY = g_iWinSizeY * 0.6f * 0.19f;
	_float fPadding = 10.f; 

	_float fTotalGridW = (3 * fCellSizeX) + (4 * fPadding);
	_float fTotalGridH = (2 * fCellSizeY) + (3 * fPadding);

	_float fStartX = m_fX - (fTotalGridW * 0.5f) + (fCellSizeX * 0.6f);
	_float fStartY = m_fY - (fTotalGridH * 0.7f) ;
	uint32_t index = 0;
	for (uint32_t i = 0; i < 4; ++i) // Rows
	{
		for (uint32_t j = 0; j < 3; ++j) // Columns
		{
			if (index == 11) {
				break;
			}
			CSearchItemFrame::ITEMFRAME_DESC pDesc;

			pDesc.pGameObjectTag = TEXT("Search_Item_Frame");
			pDesc.fSpeedPerSec = 0.f;
			pDesc.fRotationPerSec = 0.f;
			pDesc.fSizeX = fCellSizeX;
			pDesc.fSizeY = fCellSizeY;

			// Position = Start + (Index * (Size + Padding))
			pDesc.fX = fStartX + (j * (fCellSizeX + fPadding));
			pDesc.fY = fStartY + (i * (fCellSizeY + fPadding));
			
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(
				ETOUI(nextLevel),
				TEXT("Prototype_SearchItemFrame"),
				ETOUI(nextLevel),
				L"Layer_UI",
				&pDesc)))
				return E_FAIL;

			
			framePos.push_back({ pDesc.fX, pDesc.fY });
			index++;
		}
	}

	return S_OK;
}



unique_ptr<CSearchBox> CSearchBox::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSearchBox>(new CSearchBox(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSearchBox");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CSearchBox::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSearchBox(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSearchBox");
		return nullptr;
	}
	auto		pDesc = static_cast<SEARCH_DESC*>(pArg);

	if (FAILED(static_pointer_cast<CSearchBox>(pInstance)->Ready_ItemFrames(pDesc->nextLevel))) {
		MSG_BOX("Failed to Cloned : CItemFrames");
		return nullptr;
	}



	return pInstance;
}

