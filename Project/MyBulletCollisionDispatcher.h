#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <set>

class MyBulletCollisionDispatcher : public btCollisionDispatcher
{
public:
	MyBulletCollisionDispatcher(btCollisionConfiguration* config) : btCollisionDispatcher(config) {}
	void dispatchAllCollisionPairs(btOverlappingPairCache* pairCache, const btDispatcherInfo& dispatchInfo, btDispatcher* dispatcher) override;
	void Reset();

private:
	enum functionType
	{
		ENTER,
		STAY,
		EXIT,
	};
	void ObjectContactCheck(std::pair<const btCollisionObject*, const btCollisionObject*> pair, functionType fType);
	std::set<std::pair<const btCollisionObject*, const btCollisionObject*>> m_previousCollisions;

};
