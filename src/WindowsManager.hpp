#pragma once
#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include <Windows.h>
#include <vector>

class WindowsManager
{
public:
    static BOOL Init(HMODULE hmodule, const std::vector<class SignatureScanningHack *> &hacks);
    static BOOL CreateClass(LPCSTR className, WNDPROC windowProc);
    static HWND CCreateWindow(
        LPCSTR className,
        DWORD style = WS_OVERLAPPEDWINDOW,
        HWND parentWindow = NULL,
        int X = 0,
        int Y = 0,
        int width = 0,
        int height = 0,
        LPCSTR windowName = NULL,
        HMENU hmenu = NULL);
    static HWND CreateDLLWindow();
    static BOOL Exit();

private:
    static LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    static std::vector<LPCSTR> registered_window_classes;
    static std::vector<class SignatureScanningHack *> hacks;

    static HMODULE hmodule;
    static HWND DLLWindow;
    static const LPCSTR DLL_window_classname;
};

#endif