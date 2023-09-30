#include "WindowsManager.hpp"

#include "Logger.hpp"
#include "SignatureScanningHack.hpp"

const LPCSTR WindowsManager::DLL_window_classname = "My classname";

std::vector<LPCSTR> WindowsManager::registered_window_classes = {};
std::vector<SignatureScanningHack *> WindowsManager::hacks = {};

HMODULE WindowsManager::hmodule = NULL;
HWND WindowsManager::DLLWindow = NULL;

LRESULT CALLBACK WindowsManager::DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case 0:
            break;
        default:
            break;
        };
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

BOOL WindowsManager::Init(HMODULE _hmodule, const std::vector<SignatureScanningHack *> &_hacks)
{
    WindowsManager::hmodule = _hmodule;
    WindowsManager::hacks = _hacks;

    BOOL status = 1;

    status = status * CreateClass(DLL_window_classname, DLLWindowProc);
    CreateDLLWindow();

    for (auto i : hacks)
    {
        status = status * i->Init();
    }

    if (status == NULL)
    {
        Logger::Error("Failed to initialize WindowsManager");
    }

    return status;
}

BOOL WindowsManager::CreateClass(LPCSTR className, WNDPROC windowProc)
{
    WNDCLASSEX wndClass = {0};

    wndClass.lpszClassName = className;
    wndClass.hInstance = hmodule;
    wndClass.lpfnWndProc = windowProc;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpszMenuName = NULL;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    BOOL status = 1;

    if (RegisterClassEx(&wndClass) == NULL)
        status = 0;

    if (status == NULL)
    {
        Logger::Error("Failed to create " + std::string(className) + " windows class");
    }

    registered_window_classes.push_back(className);

    return status;
}

HWND WindowsManager::CCreateWindow(LPCSTR className, DWORD style, HWND parentWindow, int X, int Y, int width, int height, LPCSTR windowName, HMENU hmenu)
{
    HWND hwnd = CreateWindowEx(
        0,          // Optional window styles.
        className,  // Window class
        windowName, // Window text
        style,      // Window style

        // Size and position
        X, Y, width, height,

        parentWindow, // Parent window
        hmenu,        // Menu
        hmodule,      // Instance handle
        NULL          // Additional application data
    );

    if (hwnd == NULL)
    {
        Logger::Error("Failed to create " + std::string(className) + " window");
    }

    return hwnd;
}

HWND WindowsManager::CreateDLLWindow()
{
    if (DLLWindow == NULL)
    {
        DLLWindow = CCreateWindow(DLL_window_classname,
                                  WS_OVERLAPPEDWINDOW,
                                  NULL, CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  "WWW");

        if (DLLWindow == NULL)
        {
            Logger::Error("Failed to create DLL window");
        }
    }

    return DLLWindow;
}

BOOL WindowsManager::Exit()
{
    BOOL status = 1;

    for (auto i : registered_window_classes)
    {
        status = status * UnregisterClassA(i, hmodule);
    }

    for (auto i : hacks)
    {
        i->Exit();
        delete i;
    }

    if (status == NULL)
    {
        Logger::Error("An error occured when exiting WindowsManager");
    }

    return status;
}