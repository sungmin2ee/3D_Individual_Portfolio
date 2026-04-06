#include "Object_Manager.h"
#include "Layer.h"

#include "GameInstance.h"

CObject_Manager::~CObject_Manager()
{
}

HRESULT CObject_Manager::Initialize(uint32_t iNumLevels)
{
	m_pLayers = unique_ptr<LAYERS[]>(new LAYERS[iNumLevels]);

	m_iNumLevels = iNumLevels;

	return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (uint32_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Priority_Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (uint32_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (uint32_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Clear(uint32_t iClearLevelIndex)
{
	if (iClearLevelIndex >= m_iNumLevels ||
		nullptr == m_pLayers)
		return;

	for (auto& Pair : m_pLayers[iClearLevelIndex])
	{
		Pair.second.reset();
	}
	m_pLayers[iClearLevelIndex].clear();
}

HRESULT CObject_Manager::Add_GameObject_toLayer(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag, uint32_t iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	if (nullptr == m_pLayers ||
		iLayerLevelIndex >= m_iNumLevels)
		return E_FAIL;

	auto	pGameObject = dynamic_pointer_cast<CGameObject>(CGameInstance::Get().Clone_Prototype(iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	/* 현재 객체를 추가할려고하는 레이어가 없다면. */
	/* 새로 레이어를 만들어서 추가해준다. */
	auto		pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		auto	pNewLayer = CLayer::Create();
		if (nullptr == pNewLayer)
			return E_FAIL;

		if (FAILED(pNewLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLayerLevelIndex].emplace(strLayerTag, std::move(pNewLayer));
	}
	/* 이미 추가할려고하는 레이어가 있었다. */
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

CLayer* CObject_Manager::Find_Layer(uint32_t iLayerLevelIndex, const _wstring& strLayerTag)
{
	if (iLayerLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLayerLevelIndex].find(strLayerTag);
	if (iter == m_pLayers[iLayerLevelIndex].end())
		return nullptr;

	return iter->second.get();
}

unique_ptr<CObject_Manager> CObject_Manager::Create(uint32_t iNumLevels)
{
	auto		pInstance = unique_ptr<CObject_Manager>(new CObject_Manager());

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		return nullptr;
	}

	return pInstance;
}
