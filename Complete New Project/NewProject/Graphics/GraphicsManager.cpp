#include "GraphicsManager.h"
#include "System.h"
#include "Window.h"
#include "directx.h"
#include "VertexTypes.h"
#include "mesh.h"
#include "ForwardRenderer.h"
#include "shader.h"
#include <XInput.h>
#include "FbxImporting.h"

#include <iostream>

GraphicsManager* GraphicsManager::mInstance = 0;

extern Mesh* pTestMesh = 0;
extern Shader* pTestShader = 0;

GraphicsManager* GraphicsManager::instance() {
	if (mInstance) {
		return mInstance;
	} else {
		mInstance = new GraphicsManager();
		return mInstance;
	}
}

void GraphicsManager::deleteInstance() {
	if (mInstance != 0) {
		delete(mInstance);
		mInstance = 0;
	}
}

GraphicsManager::GraphicsManager(): System(GRAPHICS) {
    pApplicationWindow = new Window(1024, 768);
    HRESULT hr = InitDevice(pApplicationWindow->getHwnd(), &pDevice, &pContext, &pSwapChain);

    if (FAILED(hr)) {
        return;
    }

    // TODO Remove this test code
    // Create vertex buffer
    simpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
    };

    // Create index buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    initializeFbxSdk();

    pTestMesh = new Mesh();
    pTestMesh->init(pDevice, vertices, 24, indices, 36);

    std::cout << pTestMesh->dumpData();

    pCurrentRenderer = new ForwardRenderer();
    pCurrentRenderer->init(pApplicationWindow->getHwnd(), pDevice, pContext, pSwapChain );

    pTestShader = new Shader("testShader.fx", pDevice, pContext);

    XInputEnable(true);
}

GraphicsManager::~GraphicsManager() {
    delete pApplicationWindow; 
    pApplicationWindow = 0;
}

void GraphicsManager::init() {

}

void GraphicsManager::update(float dt) {
    if (!pApplicationWindow->processWindow()) {
        printf("TODO: Post quit message!");
    }

    XINPUT_STATE testState;
    XInputGetState(0,&testState);

	render(dt);
}

void GraphicsManager::destroy() {
    CleanupDevice();
    
    // Clean up the DX objects
    if (pDevice) {
        pDevice->Release();
        pDevice = NULL;
    }
    if (pContext) {
        pContext->Release();
        pContext = NULL;
    }
    if (pSwapChain) {
        pSwapChain->Release();
        pSwapChain = NULL;
    }

    delete pCurrentRenderer;
    delete pTestShader;
    delete pTestMesh;
}

void GraphicsManager::render(float dt) {
    pCurrentRenderer->render(dt, pDevice, pContext, pSwapChain);
}
