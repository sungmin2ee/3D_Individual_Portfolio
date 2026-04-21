#include "Inventory.h"
#include "ItemFrame.h"
#include "ItemIcon.h"
#include "Layer.h"
#include "EquipBorder.h"

#include "GameInstance.h"

CInventory::CInventory(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CInventory::CInventory(const CInventory& Prototype)
	: CUIObject{ Prototype }
{
}

CInventory::~CInventory()
{

}


HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	auto		pDesc = static_cast<INVENTORY_DESC*>(pArg);


	pDesc->pGameObjectTag = TEXT("Invnetory_Frame");
	pDesc->fSpeedPerSec = 0.f;
	pDesc->fRotationPerSec = 0.f;

	pDesc->fSizeX = g_iWinSizeX * 0.4f;
	pDesc->fSizeY = g_iWinSizeY * 0.7f;

	pDesc->fX = g_iWinSizeX * 0.5f;
	pDesc->fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{




	CLayer* layer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), L"UI_ICons");

	if (layer != nullptr) {
		list<shared_ptr<CGameObject>> icons = layer->GetObjects();
		if (!icons.empty()) {
			for (auto& icon : icons) {
				POINT ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);

				auto pUI = dynamic_pointer_cast<CUIObject>(icon);
				_float minX, maxX, minY, maxY;
				minX = pUI->Get_fX() - (pUI->Get_fSizeX() * 0.5f);
				maxX = pUI->Get_fX() + (pUI->Get_fSizeX() * 0.5f);
				minY = pUI->Get_fY() - (pUI->Get_fSizeY() * 0.5f);
				maxY = pUI->Get_fY() + (pUI->Get_fSizeY() * 0.5f);

				if (ptMouse.x >= minX && ptMouse.x <= maxX && ptMouse.y >= minY && ptMouse.y <= maxY) {
					if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
					CLayer* borderlayer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), L"UI_EquipBorder");
					list<shared_ptr<CGameObject>> borders = borderlayer->GetObjects();
					for (auto& border : borders) {
						auto pBorder = dynamic_pointer_cast<CUIObject>(border);
						pBorder->Set_fX(pUI->Get_fX());
						pBorder->Set_fY(pUI->Get_fY());
						pBorder->Set_Render(true);
					}
					//CGameInstance::Get().Sub_Item(pUI->Get_Tag());
					}
				}
			}
		}
	}
	

	
	__super::Priority_Update(fTimeDelta);
}

void CInventory::Update(_float fTimeDelta)
{
	/*m_fX += 20.f * fTimeDelta;
	m_fSizeY += 100.f * fTimeDelta;*/
	if (CGameInstance::Get().Key_Down(DIK_1)) {
		CGameInstance::Get().Add_Item(L"Prototype_Component_Texture_icon_lockpick");
	}
	if (CGameInstance::Get().Key_Down(DIK_2)) {
		CGameInstance::Get().Add_Item(L"Prototype_Component_Texture_icon_medicine");
	}
	if (CGameInstance::Get().Key_Down(DIK_3)) {
		CGameInstance::Get().Add_Item(L"Prototype_Component_Texture_icon_metal");
	}
	if (CGameInstance::Get().Key_Down(DIK_4)) {
		CGameInstance::Get().Sub_Item(L"Prototype_Component_Texture_icon_lockpick");
	}
	if (CGameInstance::Get().Key_Down(DIK_5)) {
		CGameInstance::Get().Sub_Item(L"Prototype_Component_Texture_icon_medicine");
	}
	if (CGameInstance::Get().Key_Down(DIK_6)) {
		CGameInstance::Get().Sub_Item(L"Prototype_Component_Texture_icon_metal");
	}
	__super::Update(fTimeDelta);
	if (CGameInstance::Get().Key_Down(DIK_E)) {
		m_bRender = !m_bRender;
	}
	if (CGameInstance::Get().Get_Changed()) {
		pair< _wstring, string> itemTag = CGameInstance::Get().Get_WhichHow();
		if (itemTag.second == "Add") {
			CItemIcon::ITEMICON_DESC pDesc;
			pDesc.bInitialRender = this->m_bRender;
			pDesc.fSizeX = g_iWinSizeX * 0.4f * 0.15f;
			pDesc.fSizeY = g_iWinSizeY * 0.7f * 0.15f;
			pDesc.fX = framePos[itemCount].first;
			pDesc.fY = framePos[itemCount].second;
			pDesc.pGameObjectTag = itemTag.first;
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER),
				L"Prototype_ItemIcon", ETOUI(LEVEL::SHELTER), L"UI_ICons", &pDesc)))
				return ;
			itemCount++;

			CGameInstance::Get().Set_Changed(false);
		}
		else if(itemTag.second == "Sub"){
			CLayer* layer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), L"UI_ICons");
			list<shared_ptr<CGameObject>>::iterator iter = layer->GetObjects().begin();
			for (iter; iter != layer->GetObjects().end();) {
				if ((*iter)->Get_Tag() == itemTag.first) {
					(*iter)->Set_Dead();
					CGameInstance::Get().Set_Changed(false);
					itemCount--;
					ReArrange();
					break;
				}
				else {
					iter++;
				}
			}
		}
	}
}

void CInventory::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CInventory));

	__super::Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
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

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CInventory::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Inventory")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CInventory::Ready_ItemFrames()
{



	_float fCellSizeX = g_iWinSizeX * 0.4f * 0.19f;
	_float fCellSizeY = g_iWinSizeY * 0.7f * 0.19f;
	_float fPadding = 10.f; 

	_float fTotalGridW = (4 * fCellSizeX) + (3 * fPadding);
	_float fTotalGridH = (3 * fCellSizeY) + (2 * fPadding);

	_float fStartX = m_fX - (fTotalGridW * 0.5f) + (fCellSizeX * 0.5f);
	_float fStartY = m_fY - (fTotalGridH * 0.6f) ;
	uint32_t index = 0;
	for (uint32_t i = 0; i < 3; ++i) // Rows
	{
		for (uint32_t j = 0; j < 4; ++j) // Columns
		{
			if (index == 11) {
				break;
			}
			CItemFrame::ITEMFRAME_DESC pDesc;

			pDesc.pGameObjectTag = TEXT("Item_Frame");
			pDesc.fSpeedPerSec = 0.f;
			pDesc.fRotationPerSec = 0.f;
			pDesc.fSizeX = fCellSizeX;
			pDesc.fSizeY = fCellSizeY;

			// Position = Start + (Index * (Size + Padding))
			pDesc.fX = fStartX + (j * (fCellSizeX + fPadding));
			pDesc.fY = fStartY + (i * (fCellSizeY + fPadding));
			
			if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(
				ETOUI(LEVEL::SHELTER),
				TEXT("Prototype_ItemFrame"),
				ETOUI(LEVEL::SHELTER),
				L"UI_Layer",
				&pDesc)))
				return E_FAIL;

			
			framePos.push_back({ pDesc.fX, pDesc.fY });
			index++;
		}
	}
	CEquipBorder::BORDER_DESC bDesc;
	bDesc.pGameObjectTag = L"EquipBorder";
	bDesc.bInitialRender = false;
	bDesc.fSizeX = fCellSizeX;
	bDesc.fSizeY = fCellSizeY;
	bDesc.fX = framePos[0].first;
	bDesc.fY = framePos[0].second;
	CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_EquipBorder", ETOUI(LEVEL::SHELTER), L"UI_EquipBorder", &bDesc);
	return S_OK;
}

void CInventory::ReArrange()
{
	CLayer* layer = CGameInstance::Get().Find_Layer(ETOUI(LEVEL::SHELTER), L"UI_ICons");
	list<shared_ptr<CGameObject>> icons = layer->GetObjects();
	uint32_t  index = 0;
	for (auto& pIcon : icons)
	{
		if (pIcon->Get_Dead())
			continue;
		auto pUI = dynamic_pointer_cast<CUIObject>(pIcon);
		if (pUI) {
			pUI->Set_fX(framePos[index].first);
			pUI->Set_fY(framePos[index].second);
			index++;
		}
	}

}

unique_ptr<CInventory> CInventory::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CInventory>(new CInventory(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CInventory::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CInventory(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		return nullptr;
	}

	if (FAILED(static_pointer_cast<CInventory>(pInstance)->Ready_ItemFrames())) {
		MSG_BOX("Failed to Cloned : CItemFrames");
		return nullptr;
	}



	return pInstance;
}

