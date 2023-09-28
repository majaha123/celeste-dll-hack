#include "InfiniteDashesHack.hpp"
#include "ProjectConfig.hpp"

#include <vector>
#include <Windows.h>
#include <sstream>

#include "MemoryHelper.hpp"
#include "Logger.hpp"
#include "WindowsManager.hpp"

#define TOGGLE 3

InfiniteDashesHack *InfiniteDashesHack::hack = NULL;

const std::vector<unsigned char> old_get_Max_Dashes_code = {
    0x83,
    0x78, 0x08, 0x00, 0x74, 0x15, 0x8B, 0x41, 0x68, 0x80, 0xB8, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x75, 0x09, 0xB8, 0x02, 0x00, 0x00, 0x00};
const std::vector<unsigned char> hacked_get_Max_Dashes_code = {
    0xEB,
    0x10,
    0x90,
    0x90,
    0x74,
    0x15,
    0x8B,
    0x41,
    0x68,
    0x80,
    0xB8,
    0x0E,
    0x01,
    0x00,
    0x00,
    0x00,
    0x75,
    0x09,
    0xB8,
    0xff,
    0xff,
    0x00,
    0x00};

bool turned_on = false;

const std::vector<int> signature = {
    0x83,
    0x78,
    0x08,
    0x00,
    0x74,
    0x15,
    0x8B,
    0x41,
    0x68,
    0x80,
    0xB8,
    0x0E,
    0x01,
    0x00,
    0x00,
    0x00,
    0x75,
    0x09,
    0xB8,
    -1,
    -1,
    -1,
    -1,
    0x8B,
    0xE5,
    0x5D,
    0xC3};

LRESULT CALLBACK InfiniteDashesHackPROC(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case TOGGLE:
            InfiniteDashesHack::getHack()->Update(NULL);
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

DWORD WINAPI findEntryThread(LPVOID params)
{
    DWORD Entry = InfiniteDashesHack::getHack()->findEntryAddress();

    DWORD status = NULL;

    if (Entry == NULL)
    {
        status = InfiniteDashesHack::NOT_FOUND;
    }
    else
    {
        status = InfiniteDashesHack::FOUND;
    }

    InfiniteDashesHack::getHack()->setStatus(status);

    std::stringstream ss;
    ss << "Result " << std::hex << (int)Entry;

    Logger::Message(ss.str());

    ExitThread(0);
}

std::string InfiniteDashesHack::getWNDClassName() const
{
    return "infinite_dashes_hack";
}

WNDPROC InfiniteDashesHack::getWNDPROC() const
{
    return InfiniteDashesHackPROC;
}

DWORD InfiniteDashesHack::findEntryAddress()
{
    Entry = MemoryHelper::GetAddressFromSignature(signature, 0x1F000000, 0x4A000000);
    if (Entry == NULL)
        Entry = MemoryHelper::GetAddressFromSignature(signature, 0x4A000000, 0x50000000);
    if (Entry == NULL)
        Entry = MemoryHelper::GetAddressFromSignature(signature);

    return Entry;
}

BOOL InfiniteDashesHack::setStatus(DWORD status)
{
    this->searchingStatus = status;

    std::string statusText = "";

    switch (status)
    {
    case NOT_FOUND:
        statusText = "Not found";
        break;
    case SEARCHING:
        statusText = "Searching";
        break;
    case FOUND:
        statusText = "Found";
        break;
    }

    SetWindowText(statusWindow, statusText.c_str());

    return 1;
}

BOOL InfiniteDashesHack::Init()
{
    BOOL status = 1;

    status = status * WindowsManager::CreateClass(getWNDClassName().c_str(), getWNDPROC());

    hwnd = WindowsManager::CCreateWindow(getWNDClassName().c_str(),
                                         WS_VISIBLE | WS_CHILD | WS_BORDER,
                                         WindowsManager::CreateDLLWindow(),
                                         10, 10, 250, 250);

    button = WindowsManager::CCreateWindow(
        "button",
        WS_VISIBLE | WS_CHILD,
        hwnd,
        25, 250 - 10, 200, 100,
        "toggle",
        (HMENU)TOGGLE);

    statusWindow = WindowsManager::CCreateWindow(
        "static",
        WS_VISIBLE | WS_CHILD,
        hwnd,
        25, 10, 200, 100,
        "Searching");

    if (hwnd == NULL || button == NULL)
    {
        status = 0;
    }

    if (status == NULL)
    {
        std::stringstream ss;
        ss << "Failed to initialize InfiniteDashesHack";
        Logger::Error(ss.str());
    }
    else
    {
        if (hack == NULL)
        {
            hack = this;
        }
    }

    setStatus(SEARCHING);

    findEntryHandle = CreateThread(NULL, 0, findEntryThread, NULL, 0, NULL);
    CloseHandle(findEntryHandle);

    return status;
}

void InfiniteDashesHack::Update(void *params)
{
    if (Entry != NULL)
    {
        std::vector<unsigned char> mem_code = turned_on ? old_get_Max_Dashes_code : hacked_get_Max_Dashes_code;

        memcpy((void *)Entry, mem_code.data(), mem_code.size());

        turned_on = !turned_on;

        if (turned_on)
        {
            Logger::Message("Turned on");
        }
        else
        {
            Logger::Message("Turned off");
        }
    }
}

void InfiniteDashesHack::Exit()
{
    if (turned_on)
    {
        Update(NULL);
    }
}
