#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <vector>
#include <unordered_map>
#include "ComponentTypes.h"

class Component;

class ComponentManager {
public:
	static ComponentManager* instance();

	void registerComponent(Component& pComponent);
	void unregisterComponent(Component& pComponent);

	Component* findComponentByID(unsigned int ID);
private:
	ComponentManager();
	~ComponentManager();

	std::unordered_map<COMPONENT_TYPES, std::vector<Component>> mMasterComponentList;

	std::vector<Component>::iterator* findComponentIteratorByID(unsigned int ID);
	static ComponentManager* mInstance;
};

#endif
