// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "ProjectConfig.hpp"
#include "SignatureScanningHack.hpp"
#include "InfiniteDashesHack.hpp"
#include "Logger.hpp"
#include "WindowsManager.hpp"

HMODULE myhModule;

DWORD WINAPI Menue()
{
    Logger::Init();

    std::vector<SignatureScanningHack *> hacks;
    hacks.push_back(new InfiniteDashesHack());

    /*while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_NUMPAD0)) // Quit
        {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1)) // find address
        {
            hacks[0]->Init();
        }
        if (GetAsyncKeyState(VK_NUMPAD2)) // toggle
        {
            hacks[0]->Update(NULL);
        }
    }*/

    BOOL status = 1;

    status = status * WindowsManager::Init(myhModule, hacks);

    if (status != NULL)
    {
        HWND hwnd = WindowsManager::CreateDLLWindow();

        if (hwnd != NULL)
        {
            ShowWindow(hwnd, SW_SHOWNORMAL);

            MSG messages;
            while (GetMessage(&messages, NULL, 0, 0))
            {
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }
        }
    }

    while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_NUMPAD0)) // Quit
        {
            break;
        }
    }

    BOOL exit_status = 1;

    exit_status = exit_status * WindowsManager::Exit();

    if (exit_status == NULL)
    {
        Logger::Error("An error occured when exiting DLL");
    }

    Logger::Exit();
    FreeLibraryAndExitThread(myhModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        myhModule = hModule;
        CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menue, NULL, 0, NULL));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
