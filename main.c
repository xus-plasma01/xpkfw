#define XUS_IMPL
#include "xpkfw.h"

int main(void) {
	xpkBegin();

	xpkWindow *window = xpkBeginWindow(0, 0, 800, 600, "window");
	xpkWindow *win2 	= xpkBeginWindow(200, 100, 800, 600, "window2");
	xpkWindow *w3 		= xpkBeginWindow(500, 500, 800, 600, "w3");

  while (1) {
    xpkSpinWindow(window);
    xpkSwapWindow(window);

    xpkSpinWindow(win2);
    xpkSwapWindow(win2);

    xpkSpinWindow(w3);
    xpkSwapWindow(w3);
  }

	xpkStopWindow(window);
	xpkStopWindow(win2);
	xpkStopWindow(w3);
  xpkEnd();
}
