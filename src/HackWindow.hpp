#pragma once
#ifndef HACK_WINDOW
#define HACK_WINDOW

class HackWindow
{
public:
    virtual void Init() {}
    virtual void Update(void *params) {}
    virtual void Exit() {}
};

#endif