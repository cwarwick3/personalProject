#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

class Window {
public:
	Window(int width, int height);
	~Window();

	bool processWindow();

	void reset();
	void changeWindowSize(int width, int height);
    inline HWND getHwnd(){return mHwnd;}
    inline int getHeight(){return mHeight;}
    inline int getWidth(){return mWidth;}
private:
	HINSTANCE mHInstance;
	HWND mHwnd;
	int mHeight;
	int mWidth;
	bool isFullscreen;
    
    // this struct holds information for the window class
    WNDCLASSEX wc;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif
