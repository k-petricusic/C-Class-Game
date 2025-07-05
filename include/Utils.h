#pragma once

#include <filesystem>
#include <string>

// Includes the correct header for the operating system
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>
#else // Linux
#include <unistd.h>
#endif

std::string get_executable_dir();
