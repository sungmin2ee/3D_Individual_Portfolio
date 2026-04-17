#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Prototype_Manager.h"
#include "Object_Manager.h"
#include "DInput_Manager.h"
#include "Collider_Manager.h"
#include "Renderer.h"
#include "CImguiMgr.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Camera.h"
#include "Helper.h"
#include "Layer.h"
#include "SaveLoad_Manager.h"
#include "PipeLine.h"

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ComPtr<ID3D11Device>& pOutDevice, ComPtr<ID3D11DeviceContext>& pOutContext)
{
    m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWinMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, pOutDevice, pOutContext);
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pPrototype_Manager)
        return E_FAIL;

    m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pObject_Manager)
        return E_FAIL;

    m_pRenderer = CRenderer::Create(pOutDevice, pOutContext);
    if (nullptr == m_pRenderer)
        return E_FAIL;

    m_pTimer_Manager = CTimer_Manager::Create();
    if (nullptr == m_pTimer_Manager)
        return E_FAIL;

    m_pLevel_Manager = CLevel_Manager::Create();
    if (nullptr == m_pLevel_Manager)
        return E_FAIL;

    m_pInput_Manager = CDInput_Manager::Create();
    if (nullptr == m_pInput_Manager)
        return E_FAIL;
    m_pInput_Manager->Ready_InputDev(EngineDesc.hInst, EngineDesc.hWnd);

    m_pModelLoader = ModelLoader::Create(EngineDesc.hWnd, pOutDevice, pOutContext);
    if (nullptr == m_pModelLoader)
        return E_FAIL;

    m_pImguiMgr = CImguiMgr::Create(EngineDesc);
    if (nullptr == m_pImguiMgr)
        return E_FAIL;
    m_pImguiMgr->Ready_Imgui(EngineDesc.hWnd, pOutDevice, pOutContext);
    
    m_pCamera = CCamera::Create();
    if (nullptr == m_pCamera)
        return E_FAIL;
    float aspect = (float)EngineDesc.iWinSizeX / EngineDesc.iWinSizeY;
    m_pCamera->SetLens(XM_PIDIV4, aspect, 0.1f, 1000.f);

    m_pHelper = CHelper::Create(EngineDesc);
    m_pCollider_Manager = Collider_Manager::Create();

    m_pSaveLoad_Manager = SaveLoad_Manager::Create(pOutDevice, pOutContext);

    m_pPipeLine = CPipeLine::Create();
    if (nullptr == m_pPipeLine)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pInput_Manager->Update_InputDev();
    if (GetAsyncKeyState('W') & 0x8000) {
        m_pCamera->Walk(10.0f * fTimeDelta);
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        m_pCamera->Walk(-10.0f * fTimeDelta);
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        m_pCamera->Strafe(-10.0f * fTimeDelta);
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        m_pCamera->Strafe(10.0f * fTimeDelta);
    }
    if (Key_Down(DIK_TAB)) {
        m_bMouse = !m_bMouse;
    }
    if (m_bMouse) {
        m_pCamera->Mouse_Move();
    }
    m_pCamera->UpdateViewMatrix();

    m_pImguiMgr->Update_Imgui();
    m_pObject_Manager->Priority_Update(fTimeDelta);

    m_pObject_Manager->Update(fTimeDelta);

    m_pObject_Manager->Late_Update(fTimeDelta);

    m_pLevel_Manager->Update(fTimeDelta);

    
}

HRESULT CGameInstance::Draw()
{
    if (FAILED(m_pRenderer->Draw()))
        return E_FAIL;

    /* 현재 장면에 그려야할 객체들의 렌더콜을 수행해준다. */
    if (FAILED(m_pLevel_Manager->Render()))
        return E_FAIL;

    if (FAILED(m_pImguiMgr->Render_Imgui()))
        return E_FAIL;
    return S_OK;
}

void CGameInstance::Clear_Resources(uint32_t iClearLevelIndex)
{
    m_pObject_Manager->Clear(iClearLevelIndex);

    m_pPrototype_Manager->Clear(iClearLevelIndex);

}

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
    m_pTimer_Manager->Set_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Ready_Timer(strTimerTag);
}
#pragma endregion

#pragma region GRAHPIC_DEVICE
HRESULT CGameInstance::Clear_BackBuffer_View(const _float4* pClearColor)
{
    return m_pGraphic_Device->Clear_BackBuffer_View(pClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
    return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
    return m_pGraphic_Device->Present();
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(uint32_t iNewLevelIndex, unique_ptr<class CLevel> pNewLevel)
{
    return m_pLevel_Manager->Change_Level(iNewLevelIndex, std::move(pNewLevel));
}
uint32_t  CGameInstance::GetCurLevelIndex() {
    return m_pLevel_Manager->GetCurLevelIndex();
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER
HRESULT CGameInstance::Add_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, unique_ptr<CPrototype> pPrototype)
{
    return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, std::move(pPrototype));
}
shared_ptr<CPrototype> CGameInstance::Clone_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
    return m_pPrototype_Manager->Clone_Prototype(iLevelIndex, strPrototypeTag, pArg);
}

CPrototype* CGameInstance::Find_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag) {
    return m_pPrototype_Manager->Find_Prototype(iLevelIndex, strPrototypeTag);
}
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_toLayer(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag, uint32_t iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
    return m_pObject_Manager->Add_GameObject_toLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);
}

CLayer* CGameInstance::Find_Layer(uint32_t iLayerLevelIndex, const _wstring& strLayerTag)
{
    return m_pObject_Manager->Find_Layer(iLayerLevelIndex, strLayerTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderObject(RENDERGROUP eRenderGroup, shared_ptr<CGameObject> pRenderObject)
{
    return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}
#pragma endregion


#pragma region MODEL_LOADER

bool CGameInstance::Load(string filename) {
    
    return m_pModelLoader->Load(filename);
}
void CGameInstance::Close () {

    return m_pModelLoader->Close();
}

vector<Mesh>& CGameInstance::Get_Meshes()
{
    return m_pModelLoader->Get_Meshes();
    // TODO: 여기에 return 문을 삽입합니다.
}

#pragma endregion

#pragma region INPUT_MANAGER
_byte	CGameInstance::Get_DIKeyState(_ubyte byKeyID) {
    return m_pInput_Manager->Get_DIKeyState(byKeyID);
}

_byte	CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse) {
    return m_pInput_Manager->Get_DIMouseState(eMouse);
}

// 현재 마우스의 특정 축 좌표를 반환
_long	CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState) {
    return m_pInput_Manager->Get_DIMouseMove(eMouseState);
}

bool CGameInstance::Key_Pressing(_ubyte byKeyID) {
    return m_pInput_Manager->Key_Pressing(byKeyID);
}
bool CGameInstance::Key_Up(_ubyte byKeyID) {
    return m_pInput_Manager->Key_Up(byKeyID);
}
bool CGameInstance::Key_Down(_ubyte byKeyID) {
    return m_pInput_Manager->Key_Down(byKeyID);
}

bool CGameInstance::Mouse_Pressing(MOUSEKEYSTATE eMouseState) {
    return m_pInput_Manager->Mouse_Pressing(eMouseState);
}
bool CGameInstance::Mouse_Up(MOUSEKEYSTATE eMouseState) {
    return m_pInput_Manager->Mouse_Up(eMouseState);
}
bool CGameInstance::Mouse_Down(MOUSEKEYSTATE eMouseState) {
    return m_pInput_Manager->Mouse_Down(eMouseState);
}
#pragma endregion


#pragma region IMGUI_MANAGER

bool CGameInstance::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return m_pImguiMgr->WinProc(hWnd, message, wParam, lParam);
}

#pragma endregion

#pragma region CAMERA
const XMFLOAT4X4 CGameInstance::GetView() {
    return m_pCamera->GetView();
}
const XMFLOAT4X4 CGameInstance::GetProj() {
    return m_pCamera->GetProj();
}
const XMMATRIX CGameInstance::GetProjXM()
{
    return m_pCamera->GetProjXM();
}
const XMMATRIX CGameInstance::GetViewXM()
{
    return m_pCamera->GetViewXM();
}
const XMFLOAT3 CGameInstance::GetPosition()
{
    return m_pCamera->GetPosition();
}
const  XMVECTOR CGameInstance::GetPositionXM() {
    return m_pCamera->GetPositionXM();
}
void CGameInstance::GetMousePointRay( _float3* pRayPos, _float3* pRayDir)
{
    m_pHelper->GetMousePointRay( pRayPos, pRayDir);
}
#pragma endregion

#pragma region COLLIDER_MANAGER

void CGameInstance::Add_Collider(weak_ptr<class CCollider> collider) {
    m_pCollider_Manager->Add_Collider(collider);
}


vector<weak_ptr<class CCollider>>& CGameInstance::Get_Colliders() {
    return m_pCollider_Manager->Get_Colliders();
}

#pragma endregion

#pragma region COLLIDER_MANAGER

HRESULT CGameInstance::Save(uint32_t level) {
    return m_pSaveLoad_Manager->Save(level);
}
HRESULT CGameInstance::Load(uint32_t level) {
    return m_pSaveLoad_Manager->Load(level);
}

#pragma endregion

#pragma region PIPELINE 
const _float4x4* CGameInstance::Get_Transform(D3DTS eState)
{
    return m_pPipeLine->Get_Transform(eState);
}
const _float4x4* CGameInstance::Get_Transform_Inverse(D3DTS eState)
{
    return m_pPipeLine->Get_Transform_Inverse(eState);
}
const _float4* CGameInstance::Get_CamPosition()
{
    return m_pPipeLine->Get_CamPosition();
}
void CGameInstance::Set_Transform(D3DTS eState, _fmatrix TransformMatrix)
{
    m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
    m_pPipeLine.reset();

    m_pRenderer.reset();

    m_pLevel_Manager.reset();
    m_pInput_Manager.reset();
    m_pTimer_Manager.reset();
    m_pCamera.reset();

    m_pObject_Manager.reset();

    m_pPrototype_Manager.reset();
    m_pModelLoader.reset();
    m_pSaveLoad_Manager.reset();
    m_pImguiMgr.reset();
    m_pGraphic_Device->Shutdown();

    m_pGraphic_Device.reset();



}

