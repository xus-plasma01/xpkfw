# xpkfw
xpkfw - a small glfw clone i made 

**NOTE: some of the features i put in this README may be _unimplemented_. 
        its because my glfw clone isnt fully complete. it takes time to 
        complete this kinda stuff.**

---------------------------------------------------------------------------------

## XPKFW DOCS (kinda)
`typedef struct xpkWindow` - the xpkWindow struct for windows

```c
xpkapi xpkError *
xpkGetError()
```
this is used for error handling.

`xpkapi int xpkBegin` - begins the lib
```c
xpkapi xpkWindow * 
xpkCreateWindow(
  int            width,
  int            height,
  const char *   title
);
```
This is the function used
for window creation.

```c
xpkapi void xpkOpenWindow(
        xpkWindow * window,
        int nCmdShow);
```
this is used for opening the window

```c
xpkapi bool
xpkWindowShouldClose(xpkWindow * window);
```
this is for loops

`xpkapi void xpkWaitEvents(xpkWindow * window);` - used for polling events
`xpkapi void xpkSwapFrames(xpkWindow * window);` - used for swapping the buffers

`void xpkDeleteWindow(xpkWindow * window);` - used for window deletion/removal
`void xpkEnd();` - ends the library
