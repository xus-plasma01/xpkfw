#ifndef XPK_H
#define XPK_H

/// === === === === === === DLL AND PLATFORM === === === === === ===
#define xpkapi

#if defined(_WIN32)
	#define XUS_WIN32
	#include <windows.h>		///< as for now it'll only support windows
#else
  #error "platform is not supported yet"
#endif ///< platform
/// === === === === === === DLL AND PLATFORM === === === === === === (end)

#ifdef __cplusplus
extern "C" {
#endif	///< __cplusplus

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

typedef struct {
#ifdef _WIN32
	HWND 				hwnd;
	HDC					hdc;
#endif
} XHWND;

typedef struct {
  int width;
  int height;
  XHWND ws;					///< if you are wondering this is 'window struct' representing XHWND
  const char *title;
  bool running;				///< needed for loop
} xpkWindow;

/* i'll work on this later
typedef struct {
  int *data;
  size_t size;
  size_t cap;
} xpkVector;
*/

typedef struct {
  DWORD code;
  char msg[512];
} xpkError;

xpkapi int xpkBegin();   	///< this goes first, it begins the library

xpkapi xpkWindow *
xpkCreateWindow(					///< this goes in second, it creates the window
  int width, 
  int height,
  const char *title);

xpkapi unsigned long
xpkGetError();

xpkapi void
xpkOpenWindow(xpkWindow *window, int nCmdShow);

xpkapi bool 
xpkWindowShouldClose(xpkWindow *window);		///< this is the loop

xpkapi void xpkWaitEvents(xpkWindow *window);
xpkapi void xpkSwapFrames(xpkWindow *window);

xpkapi void 
xpkDeleteWindow(xpkWindow *window);

xpkapi void xpkEnd();

/****************************************
 * == == == INPUT FOR KEYBOARD == == == * 
*****************************************/

// this isn't fully complete yet
#define SPACE 32
#define APOST 39

/////////////////////////////////////////

#ifdef __cplusplus
}
#endif ///< __cplusplus
#endif

#if defined(XUS_IMPL)

/// === XUS_WIN32 === ///
#ifdef XUS_WIN32
#include <windows.h>

// ERROR HANDLING
xpkapi unsigned long
xpkGetError() {
	return GetLastError();
}

// STATIC VARIABLES
static HINSTANCE 			hInst;
static bool 				xpkInitialized;					///< needed for xpkBegin and xpkEnd
static xpkError       		err;							///< needed for error handling
static int 					xpkTrue 	= 1;	///< its like in GLFW	
static int 					xpkFalse 	= 0;	///< also like in GLFW

// STATIC FUNCTIONS
static LRESULT CALLBACK WinProc(
  HWND hwnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam)
{
  switch(uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

xpkapi int xpkBegin() {
  xpkInitialized = true;
  hInst = GetModuleHandle(NULL);
  WNDCLASS wc = {};
  wc.lpfnWndProc 		= WinProc;
  wc.hInstance 			= hInst;
  wc.lpszClassName 		= "xpkWindowClass";

  if (!RegisterClassA(&wc)) {
    DWORD err = GetLastError();
    fprintf(stderr, "window class registeration failed\n");
    fprintf(stderr, "error code: %lu\n", (unsigned long)err);
    xpkEnd();
  }

  return 1;
}

xpkapi xpkWindow *
xpkCreateWindow(
  int width,
  int height,
  const char *title) 
{
  xpkBegin();
  xpkWindow *window = malloc(sizeof(*window));
  if (!window)
    return NULL;

  window->width 	= width;
  window->height 	= height;
  window->title		= title;

  window->ws.hwnd = CreateWindowExA(
    0,
    "xpkWindowClass",
    title,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    width,
    height,
    NULL,
    NULL,
    hInst,
    window);

  if (!window->ws.hwnd) {
    free(window);
    return NULL;
  }

  window->ws.hdc = GetDC(window->ws.hwnd);
  if (!window->ws.hdc) {
    DestroyWindow(window->ws.hwnd);
    free(window);
    return NULL;
  }

  return window;
}

xpkapi void
xpkOpenWindow(xpkWindow *window, int nCmdShow) {
  if (!window) {
    fprintf(stderr, "window doesn't exist");
  }

  ShowWindow(window->ws.hwnd, nCmdShow);
  UpdateWindow(window->ws.hwnd);
}

xpkapi bool 
xpkWindowShouldClose(xpkWindow *window) {
  return !window->running;
}

xpkapi void 
xpkWaitEvents(xpkWindow *window) {
  MSG msg = {};

	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
  	if (msg.message == WM_QUIT)
    	return;

  	TranslateMessage(&msg);
  	DispatchMessageA(&msg);
	}
}

xpkapi void 
xpkSwapFrames(xpkWindow *window) {
  SwapBuffers(window->ws.hdc);
}

xpkapi void 
xpkDeleteWindow(xpkWindow *window) {
  if (!window->ws.hwnd)
    return;

  if (window->ws.hdc) {
    ReleaseDC(window->ws.hwnd, window->ws.hdc);
    window->ws.hdc = NULL;
  }

  if (window->ws.hwnd) {
    DestroyWindow(window->ws.hwnd);
    window->ws.hwnd = NULL;
  }
} 

xpkapi void xpkEnd() {
  if (!xpkInitialized)
    return;

  xpkInitialized 	= false;
  hInst 			= NULL;
}

#endif
/// === XUS_WIN32 ===

#endif


