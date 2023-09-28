#pragma once
#ifndef INFINITE_DASHES_HACK
#define INFINITE_DASHES_HACK

#include "SignatureScanningHack.hpp"

class InfiniteDashesHack : public SignatureScanningHack
{
    friend DWORD WINAPI findEntryThread(LPVOID params);

    DECLARE_SIGNATURE_SCANNING_HACK_MACRO(InfiniteDashesHack, "Hack that allows to toggle infinite dashes");

public:
    static InfiniteDashesHack *getHack() { return hack; };

private:
    enum
    {
        NOT_FOUND = 0,
        SEARCHING,
        FOUND
    };

    BOOL setStatus(DWORD status);

    DWORD findEntryAddress();
    DWORD Entry;
    DWORD searchingStatus;

    HWND button;
    HWND statusWindow;

    HANDLE findEntryHandle;

    static InfiniteDashesHack *hack;
};

#endif