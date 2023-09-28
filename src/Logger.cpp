#include "Logger.hpp"

#include "ProjectConfig.hpp"
#include <iostream>
#include <Windows.h>

FILE *fp;

void Logger::Init()
{
#ifdef DEBUG_BUILD
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout); // output only
    std::cout << "test" << std::endl;
#endif
}

void Logger::Message(const std::string &message)
{
#ifdef DEBUG_BUILD
    std::cout << message << "\n";
#endif
}

void Logger::Exit()
{
#ifdef DEBUG_BUILD
    fclose(fp);
    FreeConsole();
#endif
}