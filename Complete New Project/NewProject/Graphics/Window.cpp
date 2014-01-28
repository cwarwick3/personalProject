#include "Window.h"

Window::Window(int width, int height) {
	mHInstance = (HINSTANCE)GetModuleHandle(NULL);
	mWidth = width;
	mHeight = height;

	if (mWidth <= 0) {
		mWidth = 200;
	}

	if (mHeight <= 0) {
		mHeight = 200;
	}

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = mHInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    mHwnd = CreateWindowEx(NULL,
                          "WindowClass1",    // name of the window class
                          "Our First Windowed Program",   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          300,    // x-position of the window
                          300,    // y-position of the window
                          mWidth,    // width of the window
                          mHeight,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          mHInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(mHwnd, 1);
}

Window::~Window() 
{
    UnregisterClass("WindowClass1", wc.hInstance);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}

// Handle window messages here
bool Window::processWindow() {
	// this struct holds Windows event messages
    MSG msg;

    // wait for the next message in the queue, store the result in 'msg'
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    // translate keystroke messages into the right format
    TranslateMessage(&msg);

    // send the message to the WindowProc function
    DispatchMessage(&msg);

	if (msg.wParam == WM_QUIT) {
		return false;
	} else {
		return true;
	}
}
