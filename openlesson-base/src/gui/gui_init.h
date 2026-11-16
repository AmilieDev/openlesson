#ifndef OPENLESSON_GUI_INIT_H
#define OPENLESSON_GUI_INIT_H

#include <string>

class QMainWindow;

void build_main_window(QMainWindow& window, std::string& cpu_brand);

#endif