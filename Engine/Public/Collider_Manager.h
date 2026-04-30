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

public:
	void Update();
	void Add_Collider(weak_ptr<CCollider> collider);
	void Add_ComputingCollider(weak_ptr<CCollider> collider);
	static unique_ptr<Collider_Manager> Create();
	vector<weak_ptr<CCollider>> &Get_Colliders() { return colliders; }
	vector<weak_ptr<CCollider>> &Get_ComputingColliders() { return computingCollider; }
private:
	vector<weak_ptr<CCollider>> colliders;
	vector<weak_ptr<CCollider>> computingCollider;
};

NS_END