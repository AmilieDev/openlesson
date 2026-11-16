#include "darwin.h"

#if defined(__APPLE__)

#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>

int darwin_hw_check(std::string& cpu_brand){
    cpu_brand.clear();

    char brand_buffer[256]{};
    std::size_t brand_size = sizeof(brand_buffer);

    std::cout << "[DARWIN HARDWARE] Checking for Hardware on Darwin\n";

    if(sysctlbyname("machdep.cpu.brand_string", brand_buffer, &brand_size, nullptr, 0) != 0){
        std::cerr << "[DARWIN HARDWARE] Could not read CPU brand.\n";
        return 1;
    }

    cpu_brand = brand_buffer;
    
    int isArm64 = 0;
    size_t arm_size = sizeof(isArm64);

    if (sysctlbyname("hw.optional.arm64", &isArm64, &arm_size, nullptr, 0) == 0 && isArm64 == 1) {

        cpu_brand = "Apple Silicon";
    }
    
    std::cout << "[DARWIN HARDWARE] Specified CPU: " << cpu_brand << '\n';
    return 0;
}

#endif