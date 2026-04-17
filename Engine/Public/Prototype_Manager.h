#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
/* 원형객체들을 레벨별로 구분하여 보관한다. */
/* 선택된 원형객체를 복제하여 리턴해준다. */

NS_BEGIN(Engine)

class CPrototype_Manager final
{
private:
	CPrototype_Manager();
public:
	~CPrototype_Manager();

public:
	HRESULT Initialize(uint32_t iNumLevels);
	HRESULT Add_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, unique_ptr<CPrototype> pPrototype);
	shared_ptr<CPrototype> Clone_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(uint32_t iClearLevelIndex);
	CPrototype* Find_Prototype(uint32_t iLevelIndex, const _wstring& strPrototypeTag);

private:
	uint32_t	m_iNumLevels = {};
private:
	unique_ptr<map<const _wstring, unique_ptr<CPrototype>>[]>		m_pPrototypes = { nullptr };
	typedef map<const _wstring, unique_ptr<CPrototype>>				PROTOTYPES;

private:

public:
	static unique_ptr<CPrototype_Manager> Create(uint32_t iNumLevels);
};

NS_END