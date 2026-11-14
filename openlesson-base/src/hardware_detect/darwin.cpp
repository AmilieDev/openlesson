#include <iostream>
#include <sys/sysctl.h>
#include <sys/types.h>

#include "../display_guides/display_guides.h"

int darwin_hw_check(void){    
    char cpu_brand[256];
    std::size_t size = sizeof(cpu_brand);
    
    std::cout << "[DARWIN HARDWARE] Checking for Hardware on Darwin\n";
    
    if(sysctlbyname("machdep.cpu.brand_string", cpu_brand, &size, nullptr, 0) != 0){
        std::cerr << "[DARWIN HARDWARE] Could not read CPU brand.\n";
        return 1;
    }
    
    std::cout << "[DARWIN HARDWARE] Specified CPU: " << cpu_brand << '\n';
    
    display_guide("openlesson-base/docs", cpu_brand, "desktop", "acpi");
    display_guide("openlesson-base/docs", cpu_brand, "desktop", "kernquirk");
    display_guide("openlesson-base/docs", cpu_brand, "desktop", "bootquirk");

    return 0;
    
}