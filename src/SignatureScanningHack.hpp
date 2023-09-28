#pragma once
#ifndef SIGNATURE_SCANNING_HACK
#define SIGNATURE_SCANNING_HACK

#include <Windows.h>
#include <string>

class SignatureScanningHack
{
public:
    virtual BOOL Init() { return 1; }
    virtual void Update(void *params) {}
    virtual void Exit() {}
    virtual std::string getWNDClassName() const { return ""; }
    virtual WNDPROC getWNDPROC() const { return NULL; }

protected:
    HWND hwnd;
};

#define DECLARE_SIGNATURE_SCANNING_HACK_MACRO(ClassName, Description) \
public:                                                               \
    virtual BOOL Init() override;                                     \
    virtual void Update(void *params) override;                       \
    virtual void Exit() override;                                     \
    virtual std::string getWNDClassName() const override;             \
    virtual WNDPROC getWNDPROC() const override;

#endif