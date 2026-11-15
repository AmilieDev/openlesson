#ifndef OPENLESSON_SIDEBAR_H
#define OPENLESSON_SIDEBAR_H

class QWidget;
class QListWidget;
class QLabel;

struct sidebar_widgets{
    QWidget* panel;
    QListWidget* navigation;
    QLabel* cpu_label;
};

sidebar_widgets build_sidebar(QWidget* parent);

#endif