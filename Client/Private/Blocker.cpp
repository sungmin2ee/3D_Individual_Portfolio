#include "Blocker.h"
#include "Player.h"
#include "Body_Player.h"
#include "Layer.h"

CBlocker::CBlocker(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) : CGameObject{ pDevice, pContext }
{
}

CBlocker::CBlocker(const CBlocker& Prototype) : CGameObject{ Prototype }
{
}

CBlocker::~CBlocker()
{
}

HRESULT CBlocker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlocker::Initialize(void* pArg)
{

	auto		pDesc = static_cast<BLOCKER_DESC*>(pArg);
	m_sTag = pDesc->pGameObjectTag;
	m_sLeftTag = pDesc->leftTag;
	m_sRightTag = pDesc->rightTag;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->SetWorld(pDesc->worldMat);
	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pModelCom->Calculate_Box(ETOUI(MODEL::NONANIM));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CBlocker));
	ExpandCollider();
	m_fEndPoint = { 0.4f , 1.f };
	m_fLine1Point1 = { 0.f, 0.5f };
	m_fLine1Point2 = { 0.4f, 1.f };


	return S_OK;
}

void CBlocker::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CBlocker::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pObbCom->GetSelected()) {
		ExpandCollider();
	}
	

	if (m_bDoorOpen && !m_bTransitionFinished) {
		m_fDoorOpenClose = 0.f;
		m_fShadeStart = 1.f;
		auto playerLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Player");
		if (playerLayer == nullptr) return;

		auto player = playerLayer->GetObjectFirst();
		
		auto playerPos = static_pointer_cast<CPlayer>(player)->Get_Body()->Get_Transform()->Get_State(STATE::POSITION);
		auto myPos = m_pTransformCom->Get_State(STATE::POSITION);
		_float4 deltaPos;
		XMStoreFloat4(&deltaPos, (myPos - playerPos));

		if (deltaPos.x > 0) {
			if (!m_bStartXSet) {
				m_fShadeX = 0.f;
				m_fLeftRight = 0.f;
				m_bStartXSet = true;
			}
			m_fShadeX += fTimeDelta;
			if (m_fShadeX > 1.f) {
				m_fShadeX = 1.f;
				m_bTransitionFinished = true;
				m_bStartXSet = false;
			}
		}
		else {
			if (!m_bStartXSet) {
				m_fShadeX = 1.f;
				m_fLeftRight = 1.f;
				m_bStartXSet = true;
			}
			m_fShadeX -= fTimeDelta;
			if (m_fShadeX < 0.f) {
				m_fShadeX = 0.f;
				m_bTransitionFinished = true;
				m_bStartXSet = false;


			}
		}
		return;
	}

	if (m_bDoorClose && !m_bTransitionFinished) {
		m_fDoorOpenClose = 1.f;
		m_fShadeStart = 1.f;
		auto playerLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Player");
		if (playerLayer == nullptr) return;

		auto player = playerLayer->GetObjectFirst();

		auto playerPos = static_pointer_cast<CPlayer>(player)->Get_Body()->Get_Transform()->Get_State(STATE::POSITION);
		auto myPos = m_pTransformCom->Get_State(STATE::POSITION);
		_float4 deltaPos;
		XMStoreFloat4(&deltaPos, (myPos - playerPos));
		// 플레이어가 왼쪽에 있으면
		if (deltaPos.x > 0) {
			if (!m_bStartXSet) {
				m_fShadeX = 1.f;
				m_bStartXSet = true;
				m_fLeftRight = 0.f;
			}
			m_fShadeX -= fTimeDelta;
			if (m_fShadeX < 0.f) {
				m_fShadeX = 0.f;
				m_bTransitionFinished = true;
				m_bStartXSet = false;
				m_bDoorClose = false;
				Reset();

			}
		}
		else {
			if (!m_bStartXSet) {

				m_fShadeX = 0.f; 
				m_bStartXSet = true;
				m_fLeftRight = 1.f;
			}
			m_fShadeX += fTimeDelta;
			if (m_fShadeX > 1.f) {
				m_fShadeX = 1.f;
				m_bTransitionFinished = true;
				m_bStartXSet = false;
				m_bDoorClose = false;
				Reset();
			}
		}
		return;
	}

	//if (m_bTransitionFinished) {
	//	m_bDoorClose = false;
	//	m_bDoorOpen = false;
	//}
	if (!m_bDoorClose && !m_bDoorOpen) {
		m_fShadeStart = 0.f;
		m_bStartXSet = false;

		if (m_bIsScanning) {
			if (!m_bReset) {
				Reset();
			}
			if (m_bScanningLeft) {
				if (m_fEndPoint.x < 1.f) {
					m_fEndPoint.x += fTimeDelta * 0.5f;
				}
				else {
					m_fEndPoint.y -= fTimeDelta * 0.5f;
					m_fEndPoint.x = 1.f;
					if (m_fEndPoint.y < 0.8f)
						m_fEndPoint.y = 0.8f;
				}
			}
			else {
				if (m_fEndPoint.x > 0.f) {
					m_fEndPoint.x -= fTimeDelta * 0.5f;
				}
				else {
					m_fEndPoint.y -= fTimeDelta * 0.5f;
					m_fEndPoint.x = 0.f;
					if (m_fEndPoint.y < 0.8f)
						m_fEndPoint.y = 0.8f;
				}
			}
		}
		else {
			if (m_bWasScanning) {
				if (m_bScanningLeft) {
					if (m_fEndPoint.y <= 1.f) {
						m_fEndPoint.y += fTimeDelta * 0.5f;
						if (m_fEndPoint.y >= 1.f) {
							m_fEndPoint.y = 1.f;
						}
					}
					if (m_fEndPoint.y == 1.f) {
						m_fEndPoint.x -= fTimeDelta * 0.5f;
						if (m_fEndPoint.x <= 0.f) {
							m_fEndPoint.x = 0;
							m_bWasScanning = false;
							Reset();
						}
					}
			
				}
				else {
					if (m_fEndPoint.y <= 1.f) {
						m_fEndPoint.y += fTimeDelta * 0.5f;
						if (m_fEndPoint.y >= 1.f) {
							m_fEndPoint.y = 1.f;
						}
					}
					if (m_fEndPoint.y == 1.f) {
						m_fEndPoint.x += fTimeDelta * 0.5f;
						if (m_fEndPoint.x >= 1.f) {
							m_fEndPoint.x = 1.f;
							m_bWasScanning = false;
							Reset();

						}
					}
				}
			}
	
		}
	}
}

void CBlocker::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::PRIORITY, SHARED_THIS(CBlocker));
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLOCKER , SHARED_THIS(CBlocker));
	__super::Late_Update(fTimeDelta);

}
HRESULT CBlocker::Ready_Components()
{
	//m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Blocker")));
	//if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
	//	return E_FAIL;
	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;
	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Blocker")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;
	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Blocker")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pObbBfCom = static_pointer_cast<VIBuffer_Collider>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Collider_Buffer"));
	if (nullptr == m_pObbBfCom)
	{
		MSG_BOX("OBB 버퍼 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pObbCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_OBB", &m_pObbBfCom));
	if (nullptr == m_pObbCom)
	{
		MSG_BOX("OBB 컴포넌트 클론 실패!");
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CBlocker::Render()
{
	const _float4x4* view;
	const _float4x4* proj;
	view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
	proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);


	_float4x4 mat = m_pTransformCom->GetWorld();


	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("Line1Point1", &m_fLine1Point1,sizeof _float2)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("Line1Point2", &m_fLine1Point2,sizeof _float2)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("EndPoint", &m_fEndPoint,sizeof _float2)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("ShadeStart", &m_fShadeStart, sizeof _float)))
		return E_FAIL;
	//if (m_bDoorClose || m_bDoorOpen) {
		if (FAILED(m_pShaderCom->Bind_RawValue("DoorOpenClose", &m_fDoorOpenClose, sizeof _float)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("ShadeX", &m_fShadeX, sizeof _float)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("LeftRight", &m_fLeftRight, sizeof _float)))
			return E_FAIL;
	//}

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	m_pObbCom->Render();
	return S_OK;
}


unique_ptr<CBlocker> CBlocker::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBlocker>(new CBlocker(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlocker");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBlocker::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBlocker(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlocker");
		return nullptr;
	}

	return pInstance;
}
void CBlocker::ExpandCollider()
{
	// 1. 트랜스폼에서 월드 행렬을 가져옵니다.
	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);

	// 2. Center 계산: Rect의 로컬 중심은 (0, 0, 0)입니다.
	// 따라서 오브젝트의 월드 위치(Translation)가 곧 OBB의 Center가 됩니다.
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, world);

	_float3 centerWorld;
	XMStoreFloat3(&centerWorld, vTrans);
	m_pObbCom->myOBB.Center = centerWorld;

	// 3. Extents(반지름) 계산: 
	// CVIBuffer_Rect는 기본 크기가 1x1 (로컬 -0.5 ~ 0.5)입니다.
	// Z축은 평면이므로 두께를 아주 얇게(예: 0.1f) 주거나 필요에 따라 설정합니다.
	_float3 scale = m_pTransformCom->Get_Scaled();
	m_pObbCom->myOBB.Extents.x = 1.f * 0.5f * scale.x; // 가로 반지름
	m_pObbCom->myOBB.Extents.y = 1.f * 0.5f * scale.y; // 세로 반지름
	m_pObbCom->myOBB.Extents.z = 0.01f;                 // 2D 평면이므로 최소한의 두께 제공 (충돌 감지용)

	// 4. Orientation(회전값) 추출 및 저장
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 5. 렌더링용 디버그 킹 박스(VIBuffer_Collider) 월드 행렬 갱신
	// 디버그 콜라이더 버퍼 역시 기본 크기가 1이므로 Extents * 2를 스케일로 적용합니다.
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x * 2.f,
		m_pObbCom->myOBB.Extents.y * 2.f,
		m_pObbCom->myOBB.Extents.z * 2.f);

	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(vTrans);

	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}


void CBlocker::Reset()
{
	if (m_bScanningLeft) {
		m_fEndPoint = { 0.4f,1.f };
		m_fLine1Point1 = { 0.f, 0.5f };
		m_fLine1Point2 = { 0.4f, 1.f };
	}
	else {
		m_fEndPoint = { 0.6f,1.f };
		m_fLine1Point1 = { 1.f, 0.5f };
		m_fLine1Point2 = { 0.6f, 1.f };
	}
	m_bReset = true;
}