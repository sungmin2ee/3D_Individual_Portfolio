#include "Map.h"

#include "GameInstance.h"
#include "MapPin.h"
#include "Layer.h"

CMap::CMap(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CUIObject{ pDevice, pContext }

{
}

CMap::CMap(const CMap& Prototype)
	: CUIObject{ Prototype }
{
}

CMap::~CMap()
{
}


HRESULT CMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_MapPins()))
		return E_FAIL;

	return S_OK;
}

void CMap::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CMap::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_fY = g_iWinSizeY * 0.5f;
	if (CGameInstance::Get().Key_Down(DIK_P)) {
		m_bRender = !m_bRender;
	}
	
	if (m_bRender) {
		m_fTime += (fTimeDelta/3);

		if (m_fTime > 1) {
			auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Map");
			if (layer) {
				auto mapUis = layer->GetObjects();
				for (auto ui : mapUis) {
					if (ui->Get_Tag() == L"MapPin1" || ui->Get_Tag() == L"MapPin2") {
						static_pointer_cast<CUIObject>(ui)->Set_Render(true);
					}
				}
			}
		}
		//if (m_fY > (g_iWinSizeY * 0.5f)) {
		//	m_fY -= (fTimeDelta * 700.f);
		//	if (m_fY < g_iWinSizeY * 0.5f) {
		//		m_fY = g_iWinSizeY * 0.5f;
		//		auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Map");
		//		if (layer) {
		//			auto mapUis = layer->GetObjects();
		//			for (auto ui : mapUis) {
		//				if (ui->Get_Tag() == L"MapPin1" || ui->Get_Tag() == L"MapPin2") {
		//					static_pointer_cast<CUIObject>(ui)->Set_Render(true);
		//				}
		//			}
		//		}
		//	}
		//}
	}

	if (m_bReset) {
		m_fTime = 0;
		//m_fY = g_iWinSizeY * 1.5f;
		m_bReset = false;
	}
	//if (CGameInstance::Get().Key_Down(DIK_E)) {
	//	m_bRender = !m_bRender;


}

void CMap::Late_Update(_float fTimeDelta)
{
	
	__super::Late_Update(fTimeDelta);
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::UI, SHARED_THIS(CMap));

}

HRESULT CMap::Render()
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
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 1)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_fTime, sizeof _float)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMap::Ready_Components()
{
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::SHELTER),L"Prototype_Component_Texture_Map"));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;




	return S_OK;
}

HRESULT CMap::Ready_MapPins()
{


	return S_OK;
}

unique_ptr<CMap> CMap::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CMap>(new CMap(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMap");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CMap::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CMap(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMap");
		return nullptr;
	}

	return pInstance;
}

