
#include "nt.h"

#if defined(_WIN32)

#include <iostream>
#include <windows.h>

int nt_hw_check(std::string& cpu_brand) {
    cpu_brand.clear();

    std::cout << "[NT HARDWARE] Checking for Hardware on Windows NT\n";

    char brand_buffer[256]{};
    DWORD brand_size = sizeof(brand_buffer);

    LONG result = RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_REG_SZ, nullptr, brand_buffer, &brand_size);

    if (result != ERROR_SUCCESS) {
        std::cerr << "[NT HARDWARE] Could not read CPU brand.\n";
        return 1;
    }

    cpu_brand = brand_buffer;

    std::cout << "[NT HARDWARE] Specified CPU: " << cpu_brand << '\n';

    return 0;
}

#endif
