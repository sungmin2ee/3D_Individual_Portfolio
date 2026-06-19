#pragma once

#include "Level.h"

/* 1. 새로운 레벨로 교체. */
/* 1_1. 기존 들고 있던 레벨삭제한다. */
/* 1_2. 기존 레벨용 자원을 정리해준다. */

/* 2. 현재 할당된 레벨의 반복적인 업데이트, 렌더 호출해준다. */

NS_BEGIN(Engine)

class CLevel_Manager final
{
private:
	CLevel_Manager();

public:
	~CLevel_Manager() = default;

public:
	HRESULT Change_Level(uint32_t iNewLevelIndex, unique_ptr<class CLevel> pNewLevel);
	void Update(_float fTimeDelta);
	HRESULT Render();
	uint32_t GetCurLevelIndex() { return m_iCurrentLevelIndex; }
	CLevel* Get_CurrentLevel() const { return m_pCurrentLevel.get(); }
	uint32_t &Get_DayCount() { return m_iDayCount; }
private:
	uint32_t							m_iCurrentLevelIndex = {};
	unique_ptr<class CLevel>			m_pCurrentLevel = { nullptr };
	uint32_t							m_iDayCount = 0;

public:
	static unique_ptr<CLevel_Manager> Create();
};

NS_END