#include "ComponentManager.h"
#include "Component.h"

ComponentManager* ComponentManager::mInstance = 0;

ComponentManager* ComponentManager::instance() {
	if (!mInstance) {
		mInstance = new ComponentManager();
	}
	
	return mInstance;
}

ComponentManager::ComponentManager() {

}

ComponentManager::~ComponentManager() {
	mMasterComponentList.clear();
}

// Find the storage container based on the
// component's type and then add the component
// to that container. If the container does 
// not exist, create a new container
void ComponentManager::registerComponent(Component& pComponent) {
	std::unordered_map<COMPONENT_TYPES, std::vector<Component>>::iterator container = mMasterComponentList.find(pComponent.getType());
	if (container == mMasterComponentList.end()) {
		std::vector<Component> new_container;
		new_container.push_back(pComponent);
		mMasterComponentList.insert(std::make_pair<COMPONENT_TYPES, std::vector<Component>>(pComponent.getType(), new_container));
		return;
	}

	container->second.push_back(pComponent);
	return;
}

// Search for the container then search the container for
// the exact component to remove from the list
void ComponentManager::unregisterComponent(Component& pComponent) {
	std::unordered_map<COMPONENT_TYPES, std::vector<Component>>::iterator container = mMasterComponentList.find(pComponent.getType());

	if (container != mMasterComponentList.end()) {
		std::vector<Component> componentContainer = container->second;
		std::vector<Component>::iterator it = componentContainer.begin();
		for(;it != componentContainer.end(); it++) {
			if (it->getID() == pComponent.getID()) {
				it = componentContainer.erase(it);
			}
		}
	} else {
		printf("Component does not exist inside master list, ignore unregistering");
	}
}

Component* ComponentManager::findComponentByID(unsigned int ID) {
	if (mMasterComponentList.empty()) {
		return 0;
	}

	std::unordered_map<COMPONENT_TYPES, std::vector<Component>>::iterator container = mMasterComponentList.begin();

	for(; container != mMasterComponentList.end(); container++) {
		std::vector<Component> componentContainer = container->second;
		std::vector<Component>::iterator it = componentContainer.begin();
		for(;it != componentContainer.end(); it++) {
			if (it->getID() == ID) {
				return &(*it);
			}
		}
	}

	return 0;
}
