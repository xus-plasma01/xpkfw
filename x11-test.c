#include <X11/Xlib.h>
#include <stdio.h>

int main(void)
{
  Display *dhwnd;
  Window win;
  XEvent evnt;
  int scr;

  dhwnd = XOpenDisplay(NULL);
  if (dhwnd == NULL) {
    fprintf(stderr, "cannot open display\n");
    return 1;
  }

  scr = DefaultScreen(dhwnd);
  win = XCreateSimpleWindow(
    dhwnd,
    RootWindow(dhwnd, scr),
    10,
    10,
    200,
    200,
    1,
    BlackPixel(dhwnd, scr),
    WhitePixel(dhwnd, scr)
  );

  XMapWindow(dhwnd, win);
  XSelectInput(
    dhwnd, 
    win, 
    ExposureMask 
  | KeyPressMask
  );

  while (1) {
    XNextEvent(dhwnd, &evnt);
    if (evnt.type == KeyPress)
      break;
  }

  XCloseDisplay(dhwnd);
  return 0;
}
