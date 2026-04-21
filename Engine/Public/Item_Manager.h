#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class CItem_Manager
{
private:
	CItem_Manager();
public:
	virtual ~CItem_Manager();

public:
	map<_wstring, uint32_t> &Get_Items() { return items; }
	void Add_Item(_wstring item);
	void Sub_Item(_wstring item);
	_bool &Get_Changed() { return m_bListChanged; }
	void Set_Changed(_bool flag) { m_bListChanged = flag; }
	pair< _wstring, string>&Get_WhichHow() { return itemChanged; }
public:
	static unique_ptr<CItem_Manager> Create();

private:
	HRESULT Initialize();
private:
	map<_wstring, uint32_t> items;
	_bool m_bListChanged = false;
	pair<  _wstring, string> itemChanged;
};

NS_END
