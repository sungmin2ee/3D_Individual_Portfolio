#pragma once
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Camera.h"
NS_BEGIN(Engine)

class ENGINE_DLL CHelper
{
public:
	static void GetMousePointRay(const ENGINE_DESC& EngineDesc, _float3* pRayPos, _float3* pRayDir);
};
NS_END
