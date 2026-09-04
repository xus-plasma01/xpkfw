#define XUS_IMPL
#define XUS_WIN32
#include "xpk.h"

int main(void) {
	xpkWindow *window = xpkCreateWindow(800, 600, "xpk [core] example - basic window");
	if (!window) {
		fprintf(stderr, "%c", xpkGetError();
		xpkDeleteWindow(window);
		xpkEnd();
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
