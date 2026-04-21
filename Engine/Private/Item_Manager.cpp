#include "GameInstance.h"
#include "Item_Manager.h"

CItem_Manager::CItem_Manager()
{
}
CItem_Manager::~CItem_Manager()
{
}
HRESULT CItem_Manager::Initialize()
{

	//Add_Item(L"Prototype_Item_Axe");
	//Add_Item(L"Prototype_Item_Medicine");
	//Add_Item(L"Prototype_Item_Lockpin");
	//Add_Item(L"Prototype_Item_Lumber");
	//Add_Item(L"Prototype_Item_Healing");
	//Add_Item(L"Prototype_Item_Connector");
	//Add_Item(L"Prototype_Item_Metal");
	return S_OK;
}
void CItem_Manager::Add_Item(_wstring item)
{
	if (items.empty()) {
		items.emplace(item, 1);
		m_bListChanged = true;
		itemChanged = { item, "Add" };
	}
	else {
		auto	iter = items.find(item);
		if (iter == items.end()) {
			items.emplace(item, 1);
			m_bListChanged = true;
			itemChanged = { item, "Add" };

		}
		else {
			iter->second++;
		}
		
	}
}
void CItem_Manager::Sub_Item(_wstring item)
{
	auto iter = items.find(item);

	if (iter != items.end())
	{
		iter->second--; 

		if (iter->second <= 0)
		{
			items.erase(iter); // 0개가 되면 맵에서 완전히 삭제
			m_bListChanged = true;
			itemChanged = { item, "Sub" };

		}
	}
}
unique_ptr<CItem_Manager> CItem_Manager::Create()
{
	auto	pInstance = unique_ptr<CItem_Manager>(new CItem_Manager());

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CItem_Manager");
		return nullptr;
	}

	return pInstance;
}

