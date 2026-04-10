#pragma once
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Camera.h"
NS_BEGIN(Engine)

class CHelper
{
private:
	CHelper(ENGINE_DESC EngineDesc);
public:
	virtual ~CHelper();
public:
	void GetMousePointRay(_float3* pRayPos, _float3* pRayDir);
	static unique_ptr<CHelper> Create(ENGINE_DESC EngineDesc);

private:
	ENGINE_DESC engineDesc;
};
NS_END
