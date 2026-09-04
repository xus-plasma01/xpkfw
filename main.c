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

	while (!xpkWindowShouldClose(window)) {
		xpkWaitFrames(window);
		xpkSwapFrames(window);
	}

	xpkEnd();
}
