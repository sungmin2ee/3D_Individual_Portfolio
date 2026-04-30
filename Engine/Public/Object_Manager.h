#pragma once

#include "GameObject.h"

/* 1.원형(CGameObject)를 복제한 사본객체들을 보관한다. */
/* 1_1. 레벨별로 구분해서. */
/* 1_2. 레벨안에서 사용자의 목적에 맞게 묶어서 관리한다.(CLayer)  */

NS_BEGIN(Engine)

class CObject_Manager final
{
private:
	CObject_Manager() = default;
public:
	virtual ~CObject_Manager();

public:
	HRESULT Initialize(uint32_t iNumLevels);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(uint32_t iClearLevelIndex);
	uint32_t Get_NumLevels() { return m_iNumLevels; }
public:
	HRESULT Add_GameObject_toLayer(uint32_t iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		uint32_t iLayerLevelIndex, const _wstring& strLayerTag, void* pArg);

	class CLayer* Find_Layer(uint32_t iLayerLevelIndex, const _wstring& strLayerTag);
	map<const _wstring, unique_ptr<class CLayer>>* Get_Layers() { return m_pLayers.get(); }
private:
	uint32_t		m_iNumLevels = {};
private:
	unique_ptr<map<const _wstring, unique_ptr<class CLayer>>[]>		m_pLayers = { nullptr };
	typedef map<const _wstring, unique_ptr<class CLayer>>			LAYERS;

private:

public:
	static unique_ptr<CObject_Manager> Create(uint32_t iNumLevels);
};

NS_END