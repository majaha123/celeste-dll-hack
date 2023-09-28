#pragma once
#ifndef LOGGER
#define LOGGER

#include <string>
#include <iostream>

class Logger
{
public:
    static void Init();
    static void Message(const std::string &message);
    static void Error(const std::string &message) { Message(message); };
    static void Exit();

private:
};

#endif