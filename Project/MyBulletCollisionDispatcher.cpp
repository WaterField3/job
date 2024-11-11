#include "MyBulletCollisionDispatcher.h"

#include "GameObject/GameObject.h"
#include "Component/Rigidbody.h"

using namespace TMF;

void MyBulletCollisionDispatcher::dispatchAllCollisionPairs(btOverlappingPairCache* pairCache, const btDispatcherInfo& dispatchInfo, btDispatcher* dispatcher)
{
	std::set<std::pair<const btCollisionObject*, const btCollisionObject*>> currentCollisions;

	int numManifolds = getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		auto manifold = getManifoldByIndexInternal(i);
		auto objA = manifold->getBody0();
		auto objB = manifold->getBody1();

		currentCollisions.insert(std::make_pair(objA, objB));
	}
	for (const auto& pair : currentCollisions)
	{
		if (m_previousCollisions.find(pair) == m_previousCollisions.end())
		{
			ObjectContactCheck(pair, functionType::ENTER);
		}
		else
		{
			ObjectContactCheck(pair, functionType::STAY);
		}
	}
	for (const auto& pair : m_previousCollisions)
	{
		if (currentCollisions.find(pair) == currentCollisions.end())
		{
			ObjectContactCheck(pair, functionType::EXIT);
		}
	}
	m_previousCollisions = currentCollisions;
	btCollisionDispatcher::dispatchAllCollisionPairs(pairCache, dispatchInfo, dispatcher);
}

void MyBulletCollisionDispatcher::Reset()
{
	std::set<std::pair<const btCollisionObject*, const btCollisionObject*>> currentCollisions;
	m_previousCollisions = currentCollisions;
}

void MyBulletCollisionDispatcher::ObjectContactCheck(std::pair<const btCollisionObject*, const btCollisionObject*> pair, functionType fType)
{
	auto object0 = static_cast<GameObject*>(pair.first->getUserPointer());
	auto object1 = static_cast<GameObject*>(pair.second->getUserPointer());
	auto rig0 = object0->GetComponent<Rigidbody>();
	auto rig1 = object1->GetComponent<Rigidbody>();
	switch (fType)
	{
	case MyBulletCollisionDispatcher::ENTER:
		if (!rig0.expired() && !rig1.expired())
		{
			object1->CollisionEnter(object0);
			object0->CollisionEnter(object1);
		}
		else if ((!rig0.expired() && rig1.expired()) || (rig0.expired() && rig1.expired()) || (rig0.expired() && !rig1.expired()))
		{
			object0->TrigerEnter(object1);
			object1->TrigerEnter(object0);
		}
		break;
	case MyBulletCollisionDispatcher::STAY:
		if (!rig0.expired() && !rig1.expired())
		{
			object1->CollisionStay(object0);
			object0->CollisionStay(object1);
		}
		else if ((!rig0.expired() && rig1.expired()) || (rig0.expired() && rig1.expired()) || (rig0.expired() && !rig1.expired()))
		{
			object0->TrigerStay(object1);
			object1->TrigerStay(object0);
		}
		break;
	case MyBulletCollisionDispatcher::EXIT:
		if (!rig0.expired() && !rig1.expired())
		{
			object1->CollisionExit(object0);
			object0->CollisionExit(object1);
		}
		else if ((!rig0.expired() && rig1.expired()) || (rig0.expired() && rig1.expired()))
		{
			object0->TrigerExit(object1);
			object1->TrigerExit(object0);
		}
		break;
	default:
		break;
	}
}

