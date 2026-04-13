#include "Test.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Obb.h"
#include "GameObject.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
Test::Test(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject{ pDevice, pContext }

{
}


Test::~Test()
{
}


HRESULT Test::Initialize_Prototype()
{

	return S_OK;
}

HRESULT Test::Initialize(void* pArg)
{


	/*D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);*/



	auto		pDesc = static_cast<PLAYER_DESC*>(pArg);

	pDesc->pGameObjectTag = TEXT("Player");
	pDesc->fSpeedPerSec = 10.f;
	pDesc->fRotationPerSec = 180.f;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	m_pModelCom = static_pointer_cast<Model>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Model_Joe5"));
	if (nullptr == m_pModelCom)
	{
		MSG_BOX("Player의 모델 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pShaderCom = static_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Component_Shader_VtxNonAnim"));
	if (nullptr == m_pShaderCom)
	{
		MSG_BOX("쉐이더 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pTransformCom->Scaling(0.001f, 0.001f, 0.001f);
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), 270.f);
	m_pModelCom->Calculate_Box();


	return S_OK;
}

void Test::Priority_Update(_float fTimeDelta)
{
}

void Test::Update(_float fTimeDelta)
{


}

void Test::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(Test));
}

HRESULT Test::Render()
{
	if (nullptr == m_pModelCom /*|| nullptr == m_pShaderCom*/)
		return E_FAIL;
	_float3 scale = m_pTransformCom->Get_Scaled();

	// =========================
	// Matrix 설정
	// =========================
	MatrixBuffer cb;
	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);
	//XMStoreFloat4x4(&cb.world, XMMatrixTranspose(world));
	//XMMATRIX matWorld = m_pTransformCom->m_WorldMatrix;
	//  스케일 추가 (FBX 안보일 때 필수)

   XMStoreFloat4x4(&cb.world, XMMatrixTranspose(XMMatrixIdentity()));
	cb.view = CGameInstance::Get().GetView();
	XMMATRIX matView = XMLoadFloat4x4(&cb.view);
	//XMStoreFloat4x4(&cb.view, XMMatrixTranspose(matView));

	cb.projection = CGameInstance::Get().GetProj();
	XMMATRIX matProj = XMLoadFloat4x4(&cb.projection);
	//XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(matProj));

	XMStoreFloat4x4(&cb.socket, XMMatrixIdentity());

	//m_pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &cb.world);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &cb.view);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &cb.projection);


	m_pShaderCom->Begin(0);

	//m_pShaderCom->Bind_Matrix(cb);
	m_pModelCom->Draw(m_pShaderCom.get());
	//m_pModelCom->Draw();
	return S_OK;
}
unique_ptr<Test> Test::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<Test>(new Test(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Test");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> Test::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new Test(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Test");
		return nullptr;
	}

	return pInstance;
}

