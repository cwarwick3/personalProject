#include "System.h"
#include <cassert>

SystemContainer* SystemContainer::mInstance = 0;

SystemContainer* SystemContainer::instance() {
    if (mInstance) {
        return mInstance;
    } else {
        mInstance = new SystemContainer();
        return mInstance;
    }
}

void SystemContainer::destroy() {
    if (mInstance) {
        delete mInstance;
        mInstance = 0;
    }
}

SystemContainer::SystemContainer() {
}

SystemContainer::~SystemContainer() {
}

void SystemContainer::registerSystem(System* pSystem) {
    for (int i = 0; i < mSystems.size(); i++) {
        if (mSystems[i]->GetType() == pSystem->GetType()) {
            printf("Multiple systems registered with same type");
            assert(false);
            return;
        }
    }
    mSystems.push_back(pSystem);
}

void SystemContainer::unregisterSystem(System* pSystem) {
    std::vector<System*>::iterator it = mSystems.begin();

    while (it != mSystems.end()) {
        if ((*it)->GetType() == pSystem->GetType()) {
            mSystems.erase(it);
            return;
        }
    }
}

void SystemContainer::updateSystems() {
    for (int i = 0; i < mSystems.size(); i++) {
        mSystems[i]->update(0.016f);
    }
}

System::System(int type) {
	mType = type;
    SystemContainer::instance()->registerSystem(this);
}

System::~System() {
    SystemContainer::instance()->unregisterSystem(this);
}

void System::init() {
}

void System::update(float dt) {
}

void System::destroy() {
}
