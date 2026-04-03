#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class CLayer final
{
private:
	CLayer() = default;
public:
	~CLayer() = default;

public:
	HRESULT Add_GameObject(shared_ptr<CGameObject> pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

private:
	list<shared_ptr<CGameObject>>			m_GameObjects;


public:
	static unique_ptr<CLayer> Create();

};

NS_END