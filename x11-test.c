#include <X11/Xlib.h>

int main()
{
  Display* MainDisplay = XOpenDisplay(0);
  Window RootWindow = XDefaultRootWindow(MainDisplay);
    
  int WindowX = 0;
  int WindowY = 0;
  int WindowWidth = 800;
  int WindowHeight = 600;
  int BorderWidth = 0;
  int WindowDepth = CopyFromParent;
  int WindowClass = CopyFromParent;
  Visual* WindowVisual = CopyFromParent;

  int AttributeValueMask = CWBackPixel;
  XSetWindowAttributes WindowAttributes = {};
  WindowAttributes.background_pixel = 0xffafe9af;

  Window MainWindow = XCreateWindow(MainDisplay, RootWindow, 
          WindowX, WindowY, WindowWidth, WindowHeight,
          BorderWidth, WindowDepth, WindowClass, WindowVisual,
          AttributeValueMask, &WindowAttributes);

  Window win2 = XCreateWindow(MainDisplay, RootWindow, 
            WindowX, WindowY, WindowWidth, WindowHeight,
            BorderWidth, WindowDepth, WindowClass, WindowVisual,
            AttributeValueMask, &WindowAttributes);

  XMapWindow(MainDisplay, MainWindow);
  XMapWindow(MainDisplay, win2);

  for(;;) {
      XEvent GeneralEvent = {};
      XNextEvent(MainDisplay, &GeneralEvent);
  }
}

