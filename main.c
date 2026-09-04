#define XUS_IMPL
#define XUS_WIN32
#include "xpk.h"

int main(void) {
  xpkWindow *window = xpkCreateWindow(800, 600, "thing");
  if (!window) {
    fprintf(stderr, "%c", xpkGetError());
    xpkDeleteWindow(window);
    xpkEnd();
  }

	xpkOpenWindow(window, SW_SHOW);

  while (!xpkWindowShouldClose(window)) {
    xpkWaitEvents(window);
    xpkSwapFrames(window);
  }

  xpkEnd();
}
