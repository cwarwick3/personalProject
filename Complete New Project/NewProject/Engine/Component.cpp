#include "Component.h"
#include "IDGenerator.h"
#include "ComponentManager.h"

Component::Component() {
	mID = IDGenerator::getID();
	mCompType = COMPONENT_TYPES::BLANK;
	ComponentManager::instance()->registerComponent(*this);
}

Component::~Component() {
	IDGenerator::returnID(mID);
}

Component::Component(const Component& copy) {
	this->mID = copy.mID;
	this->mCompType = copy.mCompType;
}

Component& Component::operator=(Component& rhs) {
	this->mID = rhs.mID;
	this->mCompType = rhs.mCompType;
	return *this;
}
