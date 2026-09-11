#ifndef XPK_H
#define XPK_H

/// === === === === === === DLL AND PLATFORM === === === === === ===
#ifndef xpkapi
	#define xpkapi
#endif

#if defined(_WIN32) || defined(XUS_WIN32_EXPOSE) 
	#define XUS_WIN32
	#include <windows.h>		///< as for now it'll only support windows
#elif defined(__linux__) || defined(XUS_X11_EXPOSE)
	#define XUS_X11
	#include <X11/Xlib.h>
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
#include <string.h>
#include <stdint.h>

#ifdef NDEBUG
	#include <assert.h>
#endif


typedef struct {
#if defined(_WIN32) || defined(XUS_WIN32_EXPOSE)
	HWND 				hwnd;
	HDC					hdc;
	MSG					msg;
#endif

#if defined(__linux__) || defined(XUS_X11_EXPOSE)
	Display * 	dhwnd;
	Window			win;
	Window      root;
	XEvent 			evnt;
	int					scr;
#endif
}
XHWND;


typedef struct {
  int width;
  int height;
  int posX;
  int posY;
  XHWND ws;					///< if you are wondering this is 'window struct' representing XHWND
  const char *title;
  bool running;				///< needed for loop
} 
xpkWindow;


/* i'll work on this later
typedef struct {
  int *data;
  size_t size;
  size_t cap;
} xpkVector;
*/


xpkapi int xpkBegin();   	///< this goes first, it begins the library

xpkapi xpkWindow *
xpkBeginWindow( 				///< this goes in second, it creates the window
  int positionX, 
  int positionY,
  int width,
  int height,
  const char *title);


xpkapi void xpkSpinWindow(xpkWindow *window);
xpkapi void xpkSwapWindow(xpkWindow *window);

xpkapi void 
xpkStopWindow(xpkWindow *window);

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

// STATIC VARIABLES
static HINSTANCE 	  hInst;
static bool 				xpkInitialized;					///< needed for xpkBegin and xpkEnd
static int 					xpkTrue 	= 1;					///< its like in GLFW	
static int 					xpkFalse 	= 0;					///< also like in GLFW

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

  return 0;
}

xpkapi xpkWindow *
xpkBeginWindow(
  int positionX,
  int positionY,
  int width,
  int height,
  const char *title) 
{
  xpkWindow *window = malloc(sizeof(*window));
  if (!window)
    return NULL;

  window->width 	= width;
  window->height 	= height;
  window->title		= title;
  window->posX    = positionX;
  window->posY 		= positionY;

  window->running = true;

  int x = window->posX;
  int y = window->posY;

  window->ws.hwnd = CreateWindowExA(
    0,
    "xpkWindowClass",
    title,
    WS_OVERLAPPEDWINDOW,

    // position
    x,
    y,

    // size
    width,
    height,

    // other
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

  ShowWindow(window->ws.hwnd, SW_SHOW);
  UpdateWindow(window->ws.hwnd);

  return window;
}


xpkapi void 
xpkSpinWindow(xpkWindow *window) {
  MSG msg = {};
  window->ws.msg = msg;

	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
  	if (msg.message == WM_QUIT)
    	return;

  	TranslateMessage(&msg);
  	DispatchMessageA(&msg);
	}
}

xpkapi void 
xpkSwapWindow(xpkWindow *window) {
  if (!window) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "window doesn't exist\n");
  	fprintf(stderr, "create window before using function %s at line %d\n",
  					__FUNCTION__, __LINE__);
  	xpkEnd();
  #endif
  }
  SwapBuffers(window->ws.hdc);
}

xpkapi void 
xpkStopWindow(xpkWindow *window) {
  if (!window) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "at function %s and line %d\n", 
  					__FUNCTION__, __LINE__);
    fprintf(stderr, "window doesn't exist\n");
    fprintf(stderr, "create window before destructing it\n");
  #endif
    return;
	}

  if (window->ws.hdc) {
    ReleaseDC(window->ws.hwnd, window->ws.hdc);
    window->ws.hdc = NULL;
  }

  if (window->ws.hwnd) {
    DestroyWindow(window->ws.hwnd);
    window->ws.hwnd = NULL;
  }
} 

xpkapi void 
xpkEnd() {
  if (!xpkInitialized) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "at function %s and line %d\n", __FUNCTION__, __LINE__);
  	fprintf(stderr, "xpk has not been started yet\n");
  	fprintf(stderr, "start xpk before using this function\n");
 	#endif
    return;
  }

  xpkInitialized 	= false;
  hInst 					= NULL;
}

#endif
/// === XUS_WIN32 ===

#if defined(__linux__) || defined(XUS_X11_EXPOSE)
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>

static int xpkHasBegun = 0;
static XdbeBackBuffer bbuf;
static xpkWindow *    w1;

xpkapi int
xpkBegin()
{
  xpkHasBegun = 1;

 	return 0;
}

xpkapi xpkWindow *
xpkBeginWindow(
  int positionX,
  int positionY,
  int width,
  int height,
  const char * title)
{
  xpkWindow *w = malloc(sizeof(*w));

  w->posX = positionX;
  w->posY = positionY;
  w->width = width;
  w->height = height;
  w->title = title;

	w->ws.dhwnd = XOpenDisplay(NULL);
	w->ws.scr   = DefaultScreen(w->ws.dhwnd);

	Window rwin = XDefaultRootWindow(w->ws.dhwnd);

  w->ws.win = XCreateSimpleWindow(
    w->ws.dhwnd,
    rwin,
    positionX,
    positionY,
    width,
    height,
    0,
    BlackPixel(w->ws.dhwnd, w->ws.scr),
    WhitePixel(w->ws.dhwnd, w->ws.scr)
  );

  XSelectInput(
    w->ws.dhwnd,
    w->ws.win,
    StructureNotifyMask
	);

  XMapWindow(w->ws.dhwnd, w->ws.win);

  bbuf 
  	= XdbeAllocateBackBufferName(w->ws.dhwnd, w->ws.win, XdbeUndefined);

  return w;
}

xpkapi void
xpkSpinWindow(xpkWindow *window)
{
  XEvent e;
  XNextEvent(
    window->ws.dhwnd, &e);

	window->ws.evnt = e;

  switch(e.type) {
  case StructureNotifyMask:
    window->width 	= e.xconfigure.width;
    window->height 	= e.xconfigure.height;
  }
}

xpkapi void
xpkSwapWindow(xpkWindow *window)
{
  XdbeSwapInfo i;
  i.swap_window = window->ws.win;
  i.swap_action = XdbeUndefined;
  XdbeSwapBuffers(window->ws.dhwnd, &i, 1);
}

xpkapi void
xpkStopWindow(xpkWindow *window)
{
  if (!window)
    return;

  XDestroyWindow(window->ws.dhwnd, window->ws.win);
  XdbeDeallocateBackBufferName(window->ws.dhwnd, bbuf);
  XCloseDisplay(window->ws.dhwnd);
}

xpkapi void
xpkEnd()
{
  if (w1) 
    xpkStopWindow(w1);

  xpkHasBegun = 0;
}

#endif // __linux__

#endif



