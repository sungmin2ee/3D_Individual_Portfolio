#pragma once
#include "Engine_Defines.h"
#include "Collider.h"

NS_BEGIN(Engine)
class Collider_Manager
{
private:
	Collider_Manager();
public:
	virtual ~Collider_Manager();

	void Add_Collider(weak_ptr<CCollider> collider);
	static unique_ptr<Collider_Manager> Create();
	vector<weak_ptr<CCollider>> &Get_Colliders() { return colliders; }
private:
	vector<weak_ptr<CCollider>> colliders;
};

NS_END