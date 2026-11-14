#include <iostream>

#include "hardware_detect/darwin.h"
#include "hardware_detect/linux_bsd.h"
#include "hardware_detect/nt.h"

#if defined(WIN64)
    #define OS_TYPE "winnt"
#elif __APPLE__
    #define OS_TYPE "macos"
#elif defined(__linux__)
    #define OS_TYPE "linux"
#elif defined(__FreeBSD__)
    #define OS_TYPE "freebsd"
#endif

int main(){
    if(strcmp(OS_TYPE, "winnt") == 0){
        std::cout << "[OS CHECK] Compiling for Windows\n";
    }else if(strcmp(OS_TYPE, "macos") == 0){
        std::cout << "[OS CHECK] Compiling for macOS\n";
        darwin_hw_check();
    }else if(strcmp(OS_TYPE, "linux") == 0){
        std::cout << "[OS CHECK] Compiling for Linux\n";
    }else if(strcmp(OS_TYPE, "freebsd") == 0){
        std::cout << "[OS CHECK] Compiling for FreeBSD via Linux instructions.\n";
    }
    
    std::cout << "[OpenLesson BASE] Application Compiled Successfully.\n";
    return 0;
}