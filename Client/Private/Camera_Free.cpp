#include "Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CCamera{ pDevice, pContext }

{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{
}

CCamera_Free::~CCamera_Free()
{
	int a = 10;
}


HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	auto	pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{

	
	if (CGameInstance::Get().Get_DIKeyState(DIK_UP) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (CGameInstance::Get().Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (CGameInstance::Get().Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (CGameInstance::Get().Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (CGameInstance::Get().Get_DIKeyState(DIK_C) & 0x80)
	{
		mouseMove = !mouseMove;
	}
	if (mouseMove) {
		int32_t		iMouseMove = { };

		if (iMouseMove = CGameInstance::Get().Get_DIMouseMove(DIMM::X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), iMouseMove * fTimeDelta * m_fMouseSensor);
		}

		if (iMouseMove = CGameInstance::Get().Get_DIMouseMove(DIMM::Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * m_fMouseSensor);
		}
		// 1. 화면 해상도(뷰포트 사이즈)를 가져옵니다.
		_float2 vViewportSize = CGameInstance::Get().Get_ViewportSize();

		// 2. 화면의 중앙 지점을 계산합니다. 
		// (WinAPI의 SetCursorPos는 화면 전체(Screen) 좌표 기준이므로 클라이언트 영역 좌표를 화면 좌표로 변환해야 합니다.)
		POINT ptCenter = { static_cast<LONG>(vViewportSize.x / 2.f), static_cast<LONG>(vViewportSize.y / 2.f) };

		// 3. 현재 윈도우(HWND) 기준 좌표를 모니터 전체 기준 좌표로 변환합니다.
		ClientToScreen(g_hWnd, &ptCenter); // g_hWnd는 전역 윈도우 핸들입니다.

		// 4. 커서를 중앙으로 고정합니다.
		SetCursorPos(ptCenter.x, ptCenter.y);
	}
	

	__super::Update_PipeLine();

	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{


	__super::Update(fTimeDelta);
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{


	__super::Late_Update(fTimeDelta);
}

HRESULT CCamera_Free::Render()
{

	return S_OK;
}

HRESULT CCamera_Free::Ready_Components()
{



	return S_OK;
}

unique_ptr<CCamera_Free> CCamera_Free::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CCamera_Free>(new CCamera_Free(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CCamera_Free::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CCamera_Free(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		return nullptr;
	}

	return pInstance;
}

