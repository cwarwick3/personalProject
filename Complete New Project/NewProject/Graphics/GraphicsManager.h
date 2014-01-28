#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "System.h"
#include "D3D11.h"

class Window;
class Renderer;

class GraphicsManager : public System {
public:
	static GraphicsManager* instance();
	static void deleteInstance();

	void init();
	void update(float dt);
	void destroy();
private:
	void render(float dt);

	GraphicsManager();
	~GraphicsManager();

	static GraphicsManager* mInstance;

    Window* pApplicationWindow; 
    Renderer* pCurrentRenderer;

    ID3D11Device* pDevice;
    ID3D11DeviceContext* pContext; 
    IDXGISwapChain* pSwapChain; 
    ID3D11RenderTargetView* pRenderTarget;
};

#endif
