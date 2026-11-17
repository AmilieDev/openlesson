#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include "gui/gui_init.h"

#if defined(__APPLE__)
#include "hardware_detect/darwin.h"
#elif defined(_WIN32)
#include "hardware_detect/nt.h"
#elif defined(__linux__) || defined(__FreeBSD__) || \
      defined(__OpenBSD__) || defined(__NetBSD__)
#include "hardware_detect/linux_bsd.h"
#endif

#include <string>

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    
    std::string cpu_brand;
    int result = 1;
    
    #if defined(__APPLE__)
        result = darwin_hw_check(cpu_brand);
    #elif defined(_WIN32)
        result = nt_hw_check(cpu_brand);
    #elif defined(__linux__) || defined(__FreeBSD__) || \
          defined(__OpenBSD__) || defined(__NetBSD__)
        result = linux_bsd_hw_check(cpu_brand);
    #else
        std::cerr << "Unsupported operating system\n";
        return 1;
    #endif

    QMainWindow window;
    build_main_window(window, cpu_brand);
    window.show();
    return app.exec();
}