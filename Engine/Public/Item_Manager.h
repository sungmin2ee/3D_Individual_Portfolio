#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class CItem_Manager
{
public:
	typedef struct tagItemInfoDesc 	{
		_wstring itemName;
		_wstring itemTag;
		_wstring itemDesc;
	}ITEMINFO_DESC;

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
	vector<pair< _wstring, string>>&Get_WhichHow() { return itemChanged; }
	void Clear_WhichHow();
	vector<ITEMINFO_DESC>&Get_ItemInfo() { return itemInfo; }
	vector<_wstring>& Get_FoundItem() { return foundItem; }
	void MakeRandomItem();
	
public:
	static unique_ptr<CItem_Manager> Create();

private:
	HRESULT Initialize();
private:
	map<_wstring, uint32_t> items;
	vector <_wstring> foundItem;
	_bool m_bListChanged = false;
	vector<pair<  _wstring, string>> itemChanged;
	vector<ITEMINFO_DESC> itemInfo;
};

NS_END
