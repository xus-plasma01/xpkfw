#define UNICODE
#define _UNICODE

#include <windows.h>

LRESULT CALLBACK wp(
  HWND hwnd,
  UINT msg,
  WPARAM wparam,
  LPARAM lparam)
{
  switch (msg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  default:
    return DefWindowProcW(
      hwnd,
      msg,
      wparam,
      lparam
    );
  }
}

int main(void)
{
  HINSTANCE hInstance = GetModuleHandleW(NULL);
  const wchar_t CLSNAME[] = L"Sample window class";
  WNDCLASSW wc = {0};
  wc.lpfnWndProc   = wp;
  wc.hInstance     = hInstance;
  wc.lpszClassName = CLSNAME;
    
  if (!RegisterClassW(&wc)) {
      return 1;
  }

  HWND hwnd = CreateWindowExW(
      0,
      CLSNAME,
      L"Window test",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      800,
      600,
      NULL,
      NULL,
      hInstance,
      NULL);

    if (hwnd == NULL)
      return -1;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {0};

    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

