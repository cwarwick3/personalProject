#include "frameratecontroller.h"
#include <cmath>
#include "Component.h"
#include "GraphicsManager.h"
#include "System.h"

void stressTest();

int main(void) {
	FramerateController* pFramerateController = FramerateController::instance();
    GraphicsManager* pGraphicsManager = GraphicsManager::instance();

	Component new_comp;
	Component new_comp2;

	while(pFramerateController->proceedToNextFrame()) {
		pFramerateController->logData();
        SystemContainer::instance()->updateSystems();
	}

    pGraphicsManager->destroy();
    SystemContainer::destroy();

	return 0;
}

void stressTest() {
	for (int i = 0; i < 10000000000; i++) {
		sin((float)i);
	}
}
