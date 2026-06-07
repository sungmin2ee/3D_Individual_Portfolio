#include "MapPin.h"

#include "GameInstance.h"
#include "Layer.h"
#include "MapInfo.h"

CMapPin::CMapPin(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CMapPin::CMapPin(const CMapPin& Prototype)
	: CUIObject{ Prototype }
{
}

CMapPin::~CMapPin()
{
}


HRESULT CMapPin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapPin::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
	auto		pDesc = static_cast<MAPPIN_DESC*>(pArg);

	if (FAILED(Ready_MapInfo(pDesc->textureTag)))
		return E_FAIL;

	return S_OK;
}

void CMapPin::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CMapPin::Update(_float fTimeDelta)
{
	//m_bRender = true;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_float minX, maxX, minY, maxY;
	minX = m_fX - (m_fSizeX * 0.5f);
	maxX = m_fX + (m_fSizeX * 0.5f);
	minY = m_fY - (m_fSizeY * 0.5f);
	maxY = m_fY + (m_fSizeY * 0.5f);

	if (ptMouse.x >= minX && ptMouse.x <= maxX && ptMouse.y >= minY && ptMouse.y <= maxY) {
		if (CGameInstance::Get().Mouse_Down(DIMK::LBUTTON)) {
			auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_MapInfo");
			if (layer) {
				auto mapUis = layer->GetObjects();
				for (auto& ui : mapUis) {
					if (ui->Get_Tag() == mapInfoName) {
						static_pointer_cast<CUIObject>(ui)->Set_Render(true);
						auto maplayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Map");
						auto mapUi = maplayer->GetObjects();

						for (auto& button : mapUi) {
							if (button->Get_Tag() == L"MoveButton") {
								static_pointer_cast<CUIObject>(button)->Set_Render(true);
							}
						}
					}
					else {
						static_pointer_cast<CUIObject>(ui)->Set_Render(false);
					

					}
				}
				

			}
		}
	}
	__super::Update(fTimeDelta);

}

void CMapPin::Late_Update(_float fTimeDelta)
{

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CMapPin));

	__super::Late_Update(fTimeDelta);
}

HRESULT CMapPin::Render()
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

	return S_OK;
}

HRESULT CMapPin::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER), L"Prototype_Component_Texture_MapPin"));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CMapPin::Ready_MapInfo(_wstring textureTag)
{

	CMapInfo::MAPINFO_DESC pDesc;

	pDesc.fSizeX = g_iWinSizeX * 0.25f;
	pDesc.fSizeY = g_iWinSizeY * 0.45f;
	pDesc.fX = g_iWinSizeX * 0.8f;
	pDesc.fY = g_iWinSizeY * 0.25f;
	
	pDesc.textureTag = textureTag;

	if (textureTag == L"Prototype_Component_Texture_MapInfo1") {
		pDesc.pGameObjectTag = L"MapInfo1";
		mapInfoName = L"MapInfo1";
	}
	else if (textureTag == L"Prototype_Component_Texture_MapInfo2") {
		pDesc.pGameObjectTag = L"MapInfo2";
		mapInfoName = L"MapInfo2";
	}

	if (FAILED(CGameInstance::Get().Add_GameObject_toLayer(ETOUI(LEVEL::SHELTER), L"Prototype_GameObject_MapInfo", ETOUI(LEVEL::SHELTER), TEXT("Layer_MapInfo"), &pDesc)))
		return E_FAIL;
	return S_OK;
}



unique_ptr<CMapPin> CMapPin::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CMapPin>(new CMapPin(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapPin");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CMapPin::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CMapPin(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapPin");
		return nullptr;
	}

	return pInstance;
}

