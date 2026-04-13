#include "Collider.h"
#include "GameInstance.h"

CCollider::CCollider(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext) :CComponent(pDevice, pContext)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Initialize()
{
	m_pShaderCom = static_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(0, L"Prototype_Component_Shader_Collider"));
	m_pBuffer = static_pointer_cast<VIBuffer_Cube>(CGameInstance::Get().Clone_Prototype(0, L"Prototype_Cube_Buffer"));
	return S_OK;
}
