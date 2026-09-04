#define XUS_IMPL
#define XUS_WIN32
#include "xpk.h"

int main(void) {
	/// xpkWindow * xpkCreateWindow(int width, int height, const char * title);
	/// this function creates the window 
	xpkWindow *window = xpkCreateWindow(800, 600, "xpk [core] example - basic window");

	/// for error handling
	if (!window) {
		fprintf(stderr, "%c", xpkGetError());
		xpkDeleteWindow(window);	///< deletes window bc error
		xpkEnd();					///< terminates entire lib
	}

	/// opens the window
	/// in Win32 this is
	/// ShowWindow(hwnd, SW_SHOW);
	xpkOpenWindow(window, SW_SHOW);

	/// loop
	while (!xpkWindowShouldClose(window)) {
		xpkWaitEvents(window);	///< in glfw this is "glfwPollEvents()"
		xpkSwapFrames(window);	///< in glfw this is "glfwSwapBuffers(window)"
	}

	xpkEnd();	///< terminates entire lib
}
