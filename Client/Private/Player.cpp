#include "Player.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Obb.h"
#include "GameObject.h"
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
	auto		pDesc = static_cast<PLAYER_DESC*>(pArg);
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
	m_pTransformCom->Scaling(0.001f, 0.001f, 0.001f);
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), 270.f);
	m_pModelCom->Calculate_Box();

	m_pObbCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC),L"Prototype_OBB"));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(this);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CPlayer::Update(_float fTimeDelta)
{
	if (GetAsyncKeyState('E') & 0x8000) {
		m_pTransformCom->Go_Right(fTimeDelta);
		//BoundingOrientedBox localOBB = m_LocalOBB;
		//localOBB.Transform(m_pObbCom->myOBB, worldMatrix);
		//m_pObbCom->myOBB.Transform(m_pObbCom->myOBB, );
	}
	_float fps = 1 / fTimeDelta;
	ImGui::Begin("My Second Tool");
	ImGui::Text("Current FPS: %.2f", fps);
	ImGui::Separator(); // 구분선
	if (ImGui::TreeNode("Configuration##2")) {

		/*ImGui::Begin("Operation");
		if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
			m_CurrentGizmoOperation = ImGuizmo::SCALE;*/

		ImGui::TreePop();
		ImGui::Spacing();
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	XMFLOAT4 m_vColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", (float*)&m_vColor);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
	_float3 min = m_pModelCom->GetMin();
	_float3 max = m_pModelCom->GetMax();
	_float3 scale = m_pTransformCom->Get_Scaled();
	XMVECTOR xmMin = XMLoadFloat3(&min);
	XMVECTOR xmMax = XMLoadFloat3(&max);
	XMVECTOR xmScale = XMLoadFloat3(&scale);
	XMVECTOR mid = (xmMin + (xmMax - xmMin) * 0.5);

	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);
	XMVECTOR centerWorld = XMVector3TransformCoord(mid, world);
	XMStoreFloat3(&m_pObbCom->myOBB.Center, centerWorld);

	// 2. Extents: 중심에서 각 면까지의 거리 (반지름 개념)
	// 가로 0.5m, 세로 1.0m, 두께 0.5m인 박스라면:

	m_pObbCom->myOBB.Extents = XMFLOAT3((max.x - min.x) * 0.5f * scale.x, (max.y - min.y) * 0.5f * scale.y, (max.z - min.z) * 0.5f * scale.z);

	// 3. Orientation: 회전값 (사원수)
	// 처음에는 회전이 없으므로 Identity(단위 행렬의 회전값)를 넣습니다.


	// 스케일 제거 (중요)
	XMVECTOR scale1, rot, trans;
	XMMatrixDecompose(&scale1, &rot, &trans, world);
	
	// Orientation에 넣기
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, rot);
	m_pObbCom->Update_OBB();
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CPlayer));
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pModelCom /*|| nullptr == m_pShaderCom*/)
		return E_FAIL;
	_float3 scale = m_pTransformCom->Get_Scaled();
    static float fRotation = 0.f;
    fRotation += 0.01f; // 매 프레임 조금씩 증가
    // =========================
    // Matrix 설정
    // =========================
    MatrixBuffer cb;
	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);
	 XMStoreFloat4x4(&cb.world,XMMatrixTranspose(world));
	//XMMATRIX matWorld = m_pTransformCom->m_WorldMatrix;
    //  스케일 추가 (FBX 안보일 때 필수)

   // XMStoreFloat4x4(&cb.world, XMMatrixTranspose(matWorld));
    cb.view = CGameInstance::Get().GetView();
    XMMATRIX matView = XMLoadFloat4x4(&cb.view);
    XMStoreFloat4x4(&cb.view, XMMatrixTranspose(matView));

    cb.projection = CGameInstance::Get().GetProj();
    XMMATRIX matProj = XMLoadFloat4x4(&cb.projection);
    XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(matProj));

    XMStoreFloat4x4(&cb.socket, XMMatrixIdentity());

	m_pShaderCom->Bind_Matrix(cb);
	m_pModelCom->Draw();
	m_pObbCom->Render();
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

