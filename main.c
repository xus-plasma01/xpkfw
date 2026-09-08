#define XUS_IMPL
#include "xpk.h"


int main(void) {
	xpkBegin();
  
  xpkWindow *window = xpkBeginWindow(0, 0, 800, 600, "thing");
  xpkWindow *win2 = xpkBeginWindow(100, 100, 700, 500, "thing2");
  if (!window) {
    fprintf(stderr, "failed to open window ;/\n");
    xpkStopWindow(window);
    xpkEnd();
  }

  while (!xpkWindowShouldClose(window)) {
    xpkWaitEvents(window);
    xpkSwapFrames(window);
  }

  while (!xpkWindowShouldClose(win2)) {
    xpkWaitEvents(win2);
    xpkSwapFrames(win2);
  }

  xpkEnd();
}
