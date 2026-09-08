#ifndef XPK_H
#define XPK_H

/// === === === === === === DLL AND PLATFORM === === === === === ===
#define xpkapi		///< idk what to do with this yet

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
#include <wchar.h>

typedef struct {
#if defined(_WIN32) || defined(XUS_WIN32_EXPOSE)
	HWND 				hwnd;
	HDC					hdc;
#endif

#if defined(__linux__) || defined(XUS_X11_EXPOSE)
	Display * 	dhwnd;
	Window			win;
	XEvent 			evnt;
	int					scr;
#endif
} XHWND;

typedef struct {
  int width;
  int height;
  int posX;
  int posY;
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


xpkapi int xpkBegin();   	///< this goes first, it begins the library

xpkapi xpkWindow *
xpkBeginWindow( 				///< this goes in second, it creates the window
  int positionX, 
  int positionY,
  int width,
  int height,
  const char *title);


xpkapi bool 
xpkWindowShouldClose(xpkWindow *window);		///< this is the loop

xpkapi void xpkWaitEvents(xpkWindow *window);
xpkapi void xpkSwapFrames(xpkWindow *window);

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
static xpkWindow *  window;
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
  window = malloc(sizeof(*window));
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

  if (window)
    xpkStopWindow(window);

  xpkInitialized 	= false;
  hInst 					= NULL;
}

#endif
/// === XUS_WIN32 ===

#if defined(__linux__) || defined(XUS_X11_EXPOSE)
#include <X11/Xlib.h>

static xpkWindow *xpkhwnd;

static bool xpkHasBegun;

xpkapi void
xpkEnd(void);

xpkapi int
xpkBegin(void)
{
  xpkhwnd = malloc(sizeof(*xpkhwnd));
  xpkHasBegun = true;
  
  xpkhwnd->ws.dhwnd = XOpenDisplay(NULL);
  if (!xpkhwnd->ws.dhwnd) {
    XCloseDisplay(xpkhwnd->ws.dhwnd);
    return -1;
  }

  xpkhwnd->ws.scr 
  	= DefaultScreen(xpkhwnd->ws.dhwnd);

  return 1;
}

xpkapi xpkWindow *
xpkBeginWindow(
  int 					positionX,
  int 					positionY,
  int						width,
  int						height,
  const char * 	title) 
{
  if (!xpkHasBegun) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "at function %s in line %d\n", __func__, __LINE__);
  	fprintf(stderr, "either:\n");
  	fprintf(stderr, "xpkBegin() has not been called\n");
  	fprintf(stderr, "or something else is happening.\n\n");
  #endif
  	return NULL;
  }
  xpkhwnd->width 		= width;
  xpkhwnd->height 	= height;
  xpkhwnd->title 		= title;
  xpkhwnd->posX			= positionX;
  xpkhwnd->posY			= positionY;
  xpkhwnd->running  = true;

  int x = xpkhwnd->posX;
  int y = xpkhwnd->posY;

	__auto_type root
		= RootWindow(xpkhwnd->ws.dhwnd, xpkhwnd->ws.scr);

	__auto_type bpix
		= BlackPixel(xpkhwnd->ws.dhwnd, xpkhwnd->ws.scr);

	__auto_type wpix
		= WhitePixel(xpkhwnd->ws.dhwnd, xpkhwnd->ws.scr);

  xpkhwnd->ws.win = XCreateSimpleWindow(
    xpkhwnd->ws.dhwnd,			///< display
    root,										///< parent window 
    x,											///< x
    y,											///< y
    width,									///< width
    height,									///< height
    0,											///< border width
    bpix,										///< black pixel/border color
    wpix
  );												///< white pixel/bg color

  XMapWindow(
    xpkhwnd->ws.dhwnd,
    xpkhwnd->ws.win
  );

  XSelectInput(
    xpkhwnd->ws.dhwnd,
    xpkhwnd->ws.win,
    ExposureMask
  | StructureNotifyMask
  );
    

  return xpkhwnd;
}

xpkapi bool
xpkWindowShouldClose(xpkWindow *window)
{
  return !window->running;
}

xpkapi void
xpkWaitEvents(xpkWindow *window)
{
  if (!window) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "in function %s and line %d\n", __FUNCTION__, __LINE__);
  	fprintf(stderr, "window doesn't exist yet\n");
  	fprintf(stderr, "create window before calling %s\n\n", __FUNCTION__);
 	#endif
  	return;
  }
  __auto_type e = window->ws.evnt;
  XNextEvent(window->ws.dhwnd, &e);
  switch(e.type) {
  case KeyPress:
    break;
  }
}

xpkapi void
xpkSwapFrames(xpkWindow *window)
{
  XFlush(window->ws.dhwnd);
}

xpkapi void
xpkStopWindow(xpkWindow *window)
{
  if (!window) {
  #ifdef XUS_ERRORS
  	fprintf(stderr, "at function %s and line %d\n", __FUNCTION__, __LINE__);
  	fprintf(stderr, "window hasn't been created or doesn't exist yet\n");
  	fprintf(stderr, "before window destruction, make sure to create the window\n\n");
  #endif
    return;
  }

  if (window->ws.win)
    XDestroyWindow(window->ws.dhwnd, window->ws.win);
  
  if (window->ws.dhwnd)
    XCloseDisplay(window->ws.dhwnd);

  if (window)
    free(window);
}

xpkapi void
xpkEnd(void)
{
  if (xpkhwnd)
    xpkStopWindow(xpkhwnd);

  if (xpkHasBegun == true)
    xpkHasBegun = false;
}

#endif // __linux__

#endif



