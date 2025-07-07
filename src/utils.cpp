#include "../include/Utils.h"

std::string get_executable_dir() {
    std::string result = ".";
#if defined(__APPLE__)
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        result = std::filesystem::path(path).parent_path().string();
    }
#elif defined(_WIN32)
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) > 0) {
        result = std::filesystem::path(path).parent_path().string();
    }
#else // Linux
    char path[1024];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
    if (count != -1) {
        result = std::filesystem::path(std::string(path, count)).parent_path().string();
    }
#endif
    return result;
}
