#pragma once
#ifndef INFINITE_DASHES_HACK
#define INFINITE_DASHES_HACK

#include "SignatureScanningHack.hpp"

class InfiniteDashesHack : public SignatureScanningHack
{
    friend DWORD WINAPI findEntryThread(LPVOID params);
    friend LRESULT CALLBACK InfiniteDashesHackPROC(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

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

    VOID startSearchingEntryAddress();

    BOOL setStatus(DWORD status);
    DWORD getStatus();

    BOOL setTurnedOnOff(bool turnedOn);

    BOOL initWindow();

    DWORD findEntryAddress();
    DWORD Entry;
    DWORD searchingStatus;

    HWND toggleButton;
    HWND entryButton;
    HWND statusWindow;
    HWND hackNameWindow;
    HWND turnedOnOffWindow;

    HANDLE findEntryHandle;
    HANDLE entryMutex;
    HANDLE statusMutex;

    bool turned_on;

    static InfiniteDashesHack *hack;
};

#endif