#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include "gui/gui_init.h"

#include "hardware_detect/darwin.h"
#include "hardware_detect/linux_bsd.h"
#include "hardware_detect/nt.h"

#include <string>

#if defined(WIN64)
    #define OS_TYPE "winnt"
#elif __APPLE__
    #define OS_TYPE "macos"
#elif defined(__linux__)
    #define OS_TYPE "linux"
#elif defined(__FreeBSD__)
    #define OS_TYPE "freebsd"
#endif

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    
    std::string cpu_brand;
    
    #if defined(_WIN32)
        std::cout << "[OS CHECK] Windows\n";
    #elif defined(__APPLE__)
        std::cout << "[OS CHECK] macOS\n";
        darwin_hw_check(cpu_brand);
    #elif defined(__linux__)
        std::cout << "[OS CHECK] Linux\n";
    #elif defined(__FreeBSD__)
        std::cout << "[OS CHECK] FreeBSD\n";
    #endif
    
    QMainWindow window;
    build_main_window(window, cpu_brand);
    window.show();
    return app.exec();
}