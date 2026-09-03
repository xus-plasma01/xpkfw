# xpkfw
xpkfw - a small glfw clone i made 

**NOTE: some of the features i put in this README may be _unimplemented_. 
        its because my glfw clone isnt fully complete. it takes time to 
        complete this kinda stuff.**

---------------------------------------------------------------------------------

## XPKFW DOCS (kinda)
`typedef struct xpkWindow` - the xpkWindow struct for windows


```c
xpkWindow * xpkCreateWindow(
  int            width,
  int            height,
  const char *   title
);
```
This is the function used
for window creation.


`void xpkDeleteWindow(xpkWindow * window)` - used for window deletion/removal
