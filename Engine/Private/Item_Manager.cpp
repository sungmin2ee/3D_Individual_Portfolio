#include "GameInstance.h"
#include "Item_Manager.h"

CItem_Manager::CItem_Manager()
{
}
CItem_Manager::~CItem_Manager()
{
	itemInfo.clear();
	items.clear();
}
HRESULT CItem_Manager::Initialize()
{
	itemInfo.push_back({ L"테이프", L"Prototype_Component_Texture_icon_tape", L"세상 모든 것을 고칠 수 있다는 전설의 \n접착 도구입니다.끈적거리는 잔여물이 남지만\n고정력 하나는 확실합니다." });
	itemInfo.push_back({ L"락픽", L"Prototype_Component_Texture_icon_lockpick", L"한 번만 사용할 수 있는 락픽 세트로\n누군가의 비밀이 숨겨진 잠긴 문이나\n상자를 열 때 아주 유용합니다." });
	itemInfo.push_back({ L"구급상자", L"Prototype_Component_Texture_icon_healing_kit",L"낡은 적십자 마크가 그려진 상자입니다.\n항생제와 주사기 등 전문적인 의료 용품이\n가득 들어있습니다." });
	itemInfo.push_back({ L"붕대", L"Prototype_Component_Texture_icon_bandage",  L"거친 천 조각을 깨끗하게 소독하여\n말아두었습니다.출혈을 막는 데\n가장 기본적인 도구입니다." });
	itemInfo.push_back({ L"나사", L"Prototype_Component_Texture_icon_connectors", L"녹슨 금속 조각들 사이에서 겨우 찾아낸\n온전한 나사.정밀한 장치를 조립할 때\n없어서는 안 될 부품입니다."});
	itemInfo.push_back({ L"금속", L"Prototype_Component_Texture_icon_metal",  L"버려진 기계에서 뜯어낸 날카로운 금속.\n잘 연마하면 훌륭한 칼날이 될 수도,\n방어구가 될 수도 있습니다." });
	itemInfo.push_back({ L"고기", L"Prototype_Component_Texture_icon_meat",  L"짐승에게서 떼어낸 신선한 살점입니다.\n날것으로 먹으면 탈이 날 수 있으니\n반드시 익혀 드세요." });
	itemInfo.push_back({ L"나무", L"Prototype_Component_Texture_icon_lumber",  L"마른 나뭇가지를 깎아 만든 목재입니다.\n많은 용도로 사용할수 있습니다." });
	

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

