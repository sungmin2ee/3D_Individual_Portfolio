#include "Collider_Manager.h"

Collider_Manager::Collider_Manager()
{
}

Collider_Manager::~Collider_Manager()
{
}

void Collider_Manager::Add_Collider(weak_ptr<CCollider> collider)
{
	if (collider.expired())
		return;

	colliders.push_back(collider);
}

void Collider_Manager::Add_ComputingCollider(weak_ptr<CCollider> collider)
{
	if (collider.expired())
		return;

	computingCollider.push_back(collider);
}

unique_ptr<Collider_Manager> Collider_Manager::Create()
{
	return unique_ptr<Collider_Manager>(new Collider_Manager());
}
