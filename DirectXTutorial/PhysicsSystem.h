#pragma once
#include <string>
#include <vector>
#include <map>

#include <reactphysics3d/reactphysics3d.h>
class PhysicsComponent;

class PhysicsSystem
{
public:
	//typedef std::string String;
	//typedef std::unordered_map<String, PhysicsComponent*> ComponentTable;
	typedef std::map<int, PhysicsComponent*> ComponentMap;
	

private:
	static PhysicsSystem* instance;
	
	PhysicsSystem();
	PhysicsSystem(const PhysicsSystem&) {};
	PhysicsSystem& operator=(const PhysicsSystem&) {};
	
	~PhysicsSystem();

private:
	std::vector<PhysicsComponent*> m_Components;
	ComponentMap m_Map;

	reactphysics3d::PhysicsCommon* m_PhysicsCommon;
	reactphysics3d::PhysicsWorld* m_PhysicsWorld;

public:
	static PhysicsSystem* Get();

	static void Init();
	static void Update();
	static void Release();


	void RegisterComponent(PhysicsComponent* comp);
	void UnregisterComponent(PhysicsComponent* comp);

	bool TryFindComponent(int obj_ID, PhysicsComponent* out);

	std::vector<PhysicsComponent*> GetAllComponents();
	reactphysics3d::PhysicsCommon* GetPhysicsCommon();
	reactphysics3d::PhysicsWorld* GetPhysicsWorld();
};

