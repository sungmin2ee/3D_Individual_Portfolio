#include "Player.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
CPlayer::CPlayer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext },m_pDevice{pDevice}, m_pContext{pContext}

{
}


CPlayer::~CPlayer()
{
	int a = 10;
}


HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	auto		pDesc = static_cast<BACKGROUND_DESC*>(pArg);
	m_iData = pDesc->iData;

	pDesc->pGameObjectTag = TEXT("Player");
	pDesc->fSpeedPerSec = 10.f;
	pDesc->fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	m_pModelCom = static_pointer_cast<Model>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::LOGO), L"Prototype_Player_Model"));
	if (nullptr == m_pModelCom)
	{
		MSG_BOX("Player의 모델 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pShaderCom = static_pointer_cast<Shader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::LOGO), L"Prototype_NonAnimShander"));
	if (nullptr == m_pShaderCom)
	{
		MSG_BOX("쉐이더 컴포넌트 클론 실패!");
		return E_FAIL;
	}

	/*BACKGROUND_DESC			Desc{};

	Desc.pGameObjectTag = TEXT("BackGround");
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;*/

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CPlayer::Update(_float fTimeDelta)
{
	int a = 10;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CPlayer));
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pModelCom /*|| nullptr == m_pShaderCom*/)
		return E_FAIL;
    static float fRotation = 0.f;
    fRotation += 0.01f; // 매 프레임 조금씩 증가
    // =========================
    // Matrix 설정
    // =========================
    MatrixBuffer cb;
    XMMATRIX matScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    XMMATRIX matRot = XMMatrixRotationX(XMConvertToRadians(270)); // Y축 기준 회전
   // XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(0)); // Y축 기준 회전
    XMMATRIX matTrans = XMMatrixTranslation(0.f, 0.f, 0.f);
    XMMATRIX matWorld = matScale * matRot * matTrans;
    //  스케일 추가 (FBX 안보일 때 필수)

    XMStoreFloat4x4(&cb.world, XMMatrixTranspose(matWorld));
    cb.view = CGameInstance::Get().GetView();
    XMMATRIX matView = XMLoadFloat4x4(&cb.view);
    XMStoreFloat4x4(&cb.view, XMMatrixTranspose(matView));

    cb.projection = CGameInstance::Get().GetProj();
    XMMATRIX matProj = XMLoadFloat4x4(&cb.projection);
    XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(matProj));

    XMStoreFloat4x4(&cb.socket, XMMatrixIdentity());

    //m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
    //m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //// =========================
    //// Pipeline
    //// =========================
    //m_pContext->IASetInputLayout(m_pLayout.Get());
    //m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    //m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
    //m_pContext->PSSetShader(m_pPS.Get(), 0, 0);

    //m_pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
    //context->RSSetState(m_pRasterizerState.Get());

	m_pShaderCom->Bind_Matrix(cb);
    
	m_pModelCom->Draw();

	return S_OK;
}
unique_ptr<CPlayer> CPlayer::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CPlayer>(new CPlayer(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CPlayer::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CPlayer(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		return nullptr;
	}

	return pInstance;
}

