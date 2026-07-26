# Window

## Create a simple window
```c
#pragma once
#include <Terra.h>

int main()
{
  Window* m_window = CreateWindow("Test", "PathToIcon", CENTER_POSITION, CENTER_POSITION, 700, 600, NULL);
  if (!m_window)
  {
    return 0;
  }
  MakeCurrent(m_window);

  while(!ShouldWindowClose(m_window))
  {
        /*    Code    */
      PollEvents();
      SwapBuffers(m_window);
  }

  DestoryWindow(m_window);

  return 0;
}
```

## Window Flags
```
WindowFlags_None >
WindowFlags_NoTitleBar >
WindowFlags_NoResize >
WindowFlags_NoMove >
WindowFlags_NoScrollbar >
WindowFlags_NoScrollWithMouse >
WindowFlags_NoCollapse >
WindowFlags_AlwaysAutoResize >
WindowFlags_NoBackground >
WindowFlags_NoSavedSettings >
WindowFlags_NoMouseInputs >
WindowFlags_MenuBar >
WindowFlags_HorizontalScrollbar >
WindowFlags_NoFocusOnAppearing >
WindowFlags_NoBringToFrontOnFocus >
WindowFlags_AlwaysVerticalScrollbar >
WindowFlags_AlwaysHorizontalScrollbar >
WindowFlags_NoNavInputs >
WindowFlags_NoNavFocus >
WindowFlags_UnsavedDocument >

WindowFlags_NoNav          # (WindowFlags_NoNavInputs | WindowFlags_NoNavFocus)
WindowFlags_NoDecoration   # (WindowFlags_NoTitleBar | WindowFlags_NoResize | WindowFlags_NoScrollbar | WindowFlags_NoCollapse)
WindowFlags_NoInputs       # (WindowFlags_NoMouseInputs | WindowFlags_NoNavInputs | WindowFlags_NoNavFocus)
```

## Window Modes
```
WindowMode_Minimize   #
WindowMode_Maximize   #
WindowMode_Windowed   #
WindowMode_Borderless #
```

## Window Functions
```c
Window* CreateWindow(char Title, char icon, x, y, width, height, flags, share* parent);

void DestoryWindow(Window* window);

void PollEvents();
void SwapBuffers(Window* window);
void ShowWindow(Window* window, bool show);

// Sets
void setWindowIcon();
void setWindowTitle();

// Gets
void getWindowIcon();
void getWindowTitle();
```
