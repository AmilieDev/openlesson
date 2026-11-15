#include "darwin.h"

#if defined(__APPLE__)

#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>

int darwin_hw_check(std::string& cpu_brand){
    cpu_brand.clear();

    char brand_buffer[256]{};
    std::size_t size = sizeof(brand_buffer);

    std::cout << "[DARWIN HARDWARE] Checking for Hardware on Darwin\n";

    if(sysctlbyname("machdep.cpu.brand_string", brand_buffer, &size, nullptr, 0) != 0){
        std::cerr << "[DARWIN HARDWARE] Could not read CPU brand.\n";
        return 1;
    }

    cpu_brand = brand_buffer;
    std::cout << "[DARWIN HARDWARE] Specified CPU: " << cpu_brand << '\n';
    return 0;
}

#endif