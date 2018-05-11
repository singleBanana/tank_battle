#ifndef _MAIN_H
#define _MAIN_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
void Clean();
bool FrameFunc();
bool RenderFunc();

#endif