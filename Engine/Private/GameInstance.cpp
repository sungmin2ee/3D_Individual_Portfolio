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
#include "Camera.h"
#include "Helper.h"
#include "Layer.h"
#include "SaveLoad_Manager.h"
#include "PipeLine.h"

#include "Font_Manager.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Sound_Manager.h"

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ComPtr<ID3D11Device>& pOutDevice, ComPtr<ID3D11DeviceContext>& pOutContext)
{
    m_vViewportSize = _float2(EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);


    m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWinMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, pOutDevice, pOutContext);
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    m_pThreadPool = CThreadPool::Create(thread::hardware_concurrency());
    if (nullptr == m_pThreadPool)
        return E_FAIL;
    m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pPrototype_Manager)
        return E_FAIL;

    m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pObject_Manager)
        return E_FAIL;

    m_pTarget_Manager = CTarget_Manager::Create(pOutDevice, pOutContext);
    if (nullptr == m_pTarget_Manager)
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

    m_pInput_Manager = CDInput_Manager::Create(EngineDesc.hInst,EngineDesc.hWnd);
    if (nullptr == m_pInput_Manager)
        return E_FAIL;


#ifdef DEBUG

    m_pImguiMgr = CImguiMgr::Create(EngineDesc);
    if (nullptr == m_pImguiMgr)
        return E_FAIL;
    m_pImguiMgr->Ready_Imgui(EngineDesc.hWnd, pOutDevice, pOutContext);
#endif // DEBUG

 
    


    m_pHelper = CHelper::Create(EngineDesc);
    m_pCollider_Manager = Collider_Manager::Create();

    m_pSaveLoad_Manager = SaveLoad_Manager::Create(pOutDevice, pOutContext);

    m_pPipeLine = CPipeLine::Create();
    if (nullptr == m_pPipeLine)
        return E_FAIL;
    m_pItem_Manager = CItem_Manager::Create();
    if (nullptr == m_pItem_Manager)
        return E_FAIL;
    m_pFont_Manager = CFont_Manager::Create(pOutDevice, pOutContext);
    if (nullptr == m_pFont_Manager)
        return E_FAIL;

    m_pLight_Manager = CLight_Manager::Create(pOutDevice, pOutContext);
    if (nullptr == m_pLight_Manager)
        return E_FAIL;
    m_pSound_Manager = CSound_Manager::Create();
    if (nullptr == m_pSound_Manager)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pInput_Manager->Update_InputDev();

    m_pObject_Manager->Priority_Update(fTimeDelta);
    m_pSound_Manager->UpdateSound();
    m_pPipeLine->Update();
    m_pCollider_Manager->Update();

#ifdef DEBUG
    m_pImguiMgr->Update_Imgui();

#endif // DEBUG

    m_pObject_Manager->Update(fTimeDelta);

    m_pObject_Manager->Late_Update(fTimeDelta);

    m_pLevel_Manager->Update(fTimeDelta);
  
    
}

HRESULT CGameInstance::Draw()
{
   // m_pFont_Manager->Begin();

    if (FAILED(m_pRenderer->Draw()))
        return E_FAIL;

    /* 현재 장면에 그려야할 객체들의 렌더콜을 수행해준다. */
    if (FAILED(m_pLevel_Manager->Render()))
        return E_FAIL;

    //m_pFont_Manager->End(); // Batch 끝 (실제 출력)

#ifdef DEBUG
    if (FAILED(m_pImguiMgr->Render_Imgui()))
        return E_FAIL;
#endif // DEBUG


    return S_OK;
}

void CGameInstance::Clear_Resources(uint32_t iClearLevelIndex)
{
    //CGameInstance::Get().Clear_Lights();
    m_pObject_Manager->Clear(iClearLevelIndex);

    m_pPrototype_Manager->Clear(iClearLevelIndex);

}


_float CGameInstance::Random(_float fMin, _float fMax)
{
    return fMin + static_cast<_float>(rand()) / RAND_MAX * (fMax - fMin);
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
CLevel* CGameInstance::Get_CurrentLevel() const{
    return m_pLevel_Manager->Get_CurrentLevel();
}
uint32_t CGameInstance::Get_DayCount() {
    return m_pLevel_Manager->Get_DayCount();
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
uint32_t CGameInstance::Get_NumLevels() {
    return m_pObject_Manager->Get_NumLevels();
}
map<const _wstring, unique_ptr<class CLayer>>* CGameInstance::Get_Layers() {
    return m_pObject_Manager->Get_Layers();
}
#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderObject(RENDERGROUP eRenderGroup, shared_ptr<CGameObject> pRenderObject)
{
    return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}
#ifdef _DEBUG 
HRESULT CGameInstance::Add_DebugComponent(shared_ptr<CComponent> pDebugComponent)
{
    return m_pRenderer->Add_DebugComponent(pDebugComponent);
}
#endif
#pragma endregion




#pragma endregion

#pragma region INPUT_MANAGER
_byte	CGameInstance::Get_DIKeyState(_ubyte byKeyID) {
    return m_pInput_Manager->Get_DIKeyState(byKeyID);
}

_byte	CGameInstance::Get_DIMouseState(DIMK eMouse) {
    return m_pInput_Manager->Get_DIMouseState(eMouse);
}

// 현재 마우스의 특정 축 좌표를 반환
_long	CGameInstance::Get_DIMouseMove(DIMM eMouseState) {
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

bool CGameInstance::Mouse_Pressing(DIMK eMouseState) {
    return m_pInput_Manager->Mouse_Pressing(eMouseState);
}
bool CGameInstance::Mouse_Up(DIMK eMouseState) {
    return m_pInput_Manager->Mouse_Up(eMouseState);
}
bool CGameInstance::Mouse_Down(DIMK eMouseState) {
    return m_pInput_Manager->Mouse_Down(eMouseState);
}
#pragma endregion


#pragma region IMGUI_MANAGER

bool CGameInstance::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return m_pImguiMgr->WinProc(hWnd, message, wParam, lParam);
}
void CGameInstance::Free() {
    m_pImguiMgr->Free();
}

#pragma endregion


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

#pragma region ITEM_MANAGER 
map<_wstring, uint32_t>& CGameInstance::Get_Items()
{
    return m_pItem_Manager->Get_Items();
}
void CGameInstance::Add_Item(_wstring item)
{
    return m_pItem_Manager->Add_Item(item);
}
void CGameInstance::Sub_Item(_wstring item)
{
    return m_pItem_Manager->Sub_Item(item);
}
void CGameInstance::Set_Changed(_bool flag)
{
    return m_pItem_Manager->Set_Changed(flag);
}
void CGameInstance::Clear_WhichHow()
{
    return m_pItem_Manager->Clear_WhichHow();
}
void CGameInstance::MakeRandomItem()
{
    return m_pItem_Manager->MakeRandomItem();
}
_bool CGameInstance::Get_Changed()
{
    return m_pItem_Manager->Get_Changed();
}
vector<pair< _wstring, string>>  &CGameInstance::Get_WhichHow()
{
    return m_pItem_Manager->Get_WhichHow();
}
vector<CItem_Manager::ITEMINFO_DESC>&CGameInstance::Get_ItemInfo()
{
    return m_pItem_Manager->Get_ItemInfo();
}
vector<_wstring>& CGameInstance::Get_FoundItem()
{
    return m_pItem_Manager->Get_FoundItem();
}

#pragma endregion

#pragma region FONT_MANAGER 

void CGameInstance::Begin()
{
    m_pFont_Manager->Begin();
}
void CGameInstance::End()
{
    m_pFont_Manager->End();
}
void CGameInstance::RenderText(uint32_t fontIndex, const _wstring& text, _float posX, _float posY, _vector color, _float scale)
{
    m_pFont_Manager->RenderText(fontIndex,text, posX, posY, color,scale);
}

#pragma endregion

#pragma region TARGET_MANAGER
HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, uint32_t iWidth, uint32_t iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{

    return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
    return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag)
{
    return m_pTarget_Manager->Begin_MRT(strMRTTag);
}


HRESULT CGameInstance::End_MRT()
{
    return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strTargetTag, shared_ptr<class CShader> pShader, const _char* pConstantName)
{
    return m_pTarget_Manager->Bind_ShaderResource(strTargetTag, pShader, pConstantName);
}

#ifdef _DEBUG
HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
    return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}
HRESULT CGameInstance::Debug_RT_Render(const _wstring& strMRTTag, shared_ptr<class CShader> pShader, const _char* pConstantName, shared_ptr<class CVIBuffer_Rect> pVIBuffer)
{
    return m_pTarget_Manager->Debug_Render(strMRTTag, pShader, pConstantName, pVIBuffer);
}
#endif
#pragma endregion

#pragma region LIGHT_MANAGER

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
    return m_pLight_Manager->Add_Light(LightDesc);
}
HRESULT CGameInstance::Render_Lights(shared_ptr<class CShader> pShader, shared_ptr<class CVIBuffer_Rect> pVIBuffer)
{
    return m_pLight_Manager->Render(pShader, pVIBuffer);
}
void  CGameInstance::Clear_Lights()
{
    return m_pLight_Manager->Clear_Lights();
}
list<shared_ptr<class CLight>> &CGameInstance::Get_Lights()
{
    return m_pLight_Manager->Get_Lights();
}
#pragma endregion

#pragma region SOUND_MANAGER

void  CGameInstance::PlaySoundLoop(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume)
{
    return m_pSound_Manager->PlaySoundLoop(svSoundKey, eID, fVolume);
}
void  CGameInstance::PlaySoundOne(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume)
{
    return m_pSound_Manager->PlaySoundOne(svSoundKey, eID, fVolume);
}
void  CGameInstance::PlaySoundLoop(const TCHAR* soundKey, FMOD_CHANNEL** ppChannel, _float volume)
{
    return m_pSound_Manager->PlaySoundLoop(soundKey, ppChannel, volume);
}
void  CGameInstance::PlaySoundOne(const TCHAR* soundKey, FMOD_CHANNEL** ppChannel, _float volume)
{
    return m_pSound_Manager->PlaySoundOne(soundKey, ppChannel, volume);
}
void  CGameInstance::PlaySound(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume)
{
    return m_pSound_Manager->PlaySoundLoop(svSoundKey, eID, fVolume);
}
void  CGameInstance::PlayBGM(const TCHAR* svSoundKey, _float fVolume)
{
    return m_pSound_Manager->PlayBGM(svSoundKey, fVolume);
}
void  CGameInstance::StopSound(CHANNELID eID)
{
    return m_pSound_Manager->StopSound(eID);
}
void  CGameInstance::StopSound(FMOD_CHANNEL** ppChannel)
{
    return m_pSound_Manager->StopSound(ppChannel);
}
void  CGameInstance::StopAll()
{
    return m_pSound_Manager->StopAll();
}
void  CGameInstance::SetChannelVolume(CHANNELID eID, _float fVolume)
{
    return m_pSound_Manager->SetChannelVolume(eID, fVolume);
}
void  CGameInstance::SetChannelVolume(FMOD_CHANNEL** ppChannel, _float fVolume)
{
    return m_pSound_Manager->SetChannelVolume(ppChannel, fVolume);
}


#pragma endregion

#pragma region THREAD_POOL

CThreadPool* CGameInstance::Get_ThreadPool()
{
    return m_pThreadPool.get();
}

void  CGameInstance::Enqueue(function<void()> job)
{
    return m_pThreadPool->Enqueue(job);
}
void  CGameInstance::WaitAll()
{
    return m_pThreadPool->WaitAll();
}

#pragma endregion
void CGameInstance::Release_Engine()
{



    m_pLight_Manager.reset();
    m_pTarget_Manager.reset();
    m_pHelper.reset();
    m_pFont_Manager.reset();
    m_pItem_Manager.reset();
    m_pSaveLoad_Manager.reset();
    m_pCollider_Manager.reset();

    m_pRenderer.reset();

    m_pLevel_Manager.reset();
    m_pSound_Manager.reset();

    m_pTimer_Manager.reset();

    m_pObject_Manager.reset();
    m_pThreadPool.reset();
    m_pPrototype_Manager.reset();
    m_pInput_Manager.reset();
#ifdef DEBUG
    m_pImguiMgr->Free();
    m_pImguiMgr.reset();

#endif // DEBUG


    m_pPipeLine.reset();
    m_pGraphic_Device->Shutdown();

    m_pGraphic_Device.reset();

}

