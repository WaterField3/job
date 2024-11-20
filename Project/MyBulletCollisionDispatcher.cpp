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
	auto pGameObject0 = static_cast<GameObject*>(pair.first->getUserPointer());
	auto pGameObject1 = static_cast<GameObject*>(pair.second->getUserPointer());
	auto pRigidbody0 = pGameObject0->GetComponent<Rigidbody>();
	auto pRigidbody1 = pGameObject1->GetComponent<Rigidbody>();
	switch (fType)
	{
	case MyBulletCollisionDispatcher::ENTER:
		if (!pRigidbody0.expired() && !pRigidbody1.expired())
		{
			pGameObject1->CollisionEnter(pGameObject0);
			pGameObject0->CollisionEnter(pGameObject1);
		}
		else if ((!pRigidbody0.expired() && pRigidbody1.expired()) || (pRigidbody0.expired() && pRigidbody1.expired()) || (pRigidbody0.expired() && !pRigidbody1.expired()))
		{
			pGameObject0->TrigerEnter(pGameObject1);
			pGameObject1->TrigerEnter(pGameObject0);
		}
		break;
	case MyBulletCollisionDispatcher::STAY:
		if (!pRigidbody0.expired() && !pRigidbody1.expired())
		{
			pGameObject1->CollisionStay(pGameObject0);
			pGameObject0->CollisionStay(pGameObject1);
		}
		else if ((!pRigidbody0.expired() && pRigidbody1.expired()) || (pRigidbody0.expired() && pRigidbody1.expired()) || (pRigidbody0.expired() && !pRigidbody1.expired()))
		{
			pGameObject0->TrigerStay(pGameObject1);
			pGameObject1->TrigerStay(pGameObject0);
		}
		break;
	case MyBulletCollisionDispatcher::EXIT:
		if (!pRigidbody0.expired() && !pRigidbody1.expired())
		{
			pGameObject1->CollisionExit(pGameObject0);
			pGameObject0->CollisionExit(pGameObject1);
		}
		else if ((!pRigidbody0.expired() && pRigidbody1.expired()) || (pRigidbody0.expired() && pRigidbody1.expired()))
		{
			pGameObject0->TrigerExit(pGameObject1);
			pGameObject1->TrigerExit(pGameObject0);
		}
		break;
	default:
		break;
	}
}

