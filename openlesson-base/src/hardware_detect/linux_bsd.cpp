
#include "linux_bsd.h"

#if defined(__linux__) || defined(__FreeBSD__) || \
    defined(__OpenBSD__) || defined(__NetBSD__)

#include <iostream>
#include <fstream>
#include <string>

#if !defined(__linux__)

#include <sys/types.h>
#include <sys/sysctl.h>

#endif

int linux_bsd_hw_check(std::string& cpu_brand) {
    cpu_brand.clear();

    std::cout << "[LINUX/BSD HARDWARE] Checking CPU hardware\n";

#if defined(__linux__)

    std::ifstream cpuinfo("/proc/cpuinfo");

    if (!cpuinfo.is_open()) {
        std::cerr << "[LINUX HARDWARE] Cannot open /proc/cpuinfo\n";
        return 1;
    }

    std::string line;

    while (std::getline(cpuinfo, line)) {
        auto pos = line.find(':');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        auto trim = [](std::string& str) {
            auto start = str.find_first_not_of(" \t");
            if (start == std::string::npos) {
                str.clear();
                return;
            }

            auto end = str.find_last_not_of(" \t\r\n");
            str = str.substr(start, end - start + 1);
        };

        trim(key);
        trim(value);

        if (key == "model name" || key == "Hardware" || key == "Processor") {
            cpu_brand = value;
            break;
        }
    }

#else

    char brand_buffer[256]{};
    size_t brand_size = sizeof(brand_buffer);

    const char* keys[] = {
#if defined(__NetBSD__)
        "machdep.cpu_brand",
#endif
        "hw.model",
        "hw.machine"
    };

    for (const char* key : keys) {
        brand_size = sizeof(brand_buffer);
        brand_buffer[0] = '\0';

        if (sysctlbyname(key, brand_buffer, &brand_size, nullptr, 0) == 0 && brand_buffer[0] != '\0') {
            cpu_brand = brand_buffer;
            break;
        }
    }

#endif

    if (cpu_brand.empty()) {
        std::cerr << "[LINUX/BSD HARDWARE] Could not detect CPU.\n";
        return 1;
    }

    std::cout << "[LINUX/BSD HARDWARE] Specified CPU: " << cpu_brand << '\n';

    return 0;
}

#endif
