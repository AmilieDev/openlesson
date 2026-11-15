#include "sidebar.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

sidebar_widgets build_sidebar(QWidget* parent){
    auto* panel = new QWidget(parent);
    panel->setObjectName("sidebar");
    panel->setFixedWidth(175);
    
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(10, 12, 10, 12);
    layout->setSpacing(8);
    
    auto* title = new QLabel("OpenLesson", panel);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    auto* navigation = new QListWidget(panel);
    navigation->setObjectName("sidebar_navigation");
    navigation->addItems({
        "Downloading macOS",
        "Preparing your EFI",
        "Preparing to reboot",
        "Troubleshooting"
    });
    layout->addWidget(navigation, 1);
    
    auto* cpu_heading = new QLabel("Detected CPU", panel);
    cpu_heading->setAlignment(Qt::AlignCenter);
    layout->addWidget(cpu_heading);

    auto* cpu_label = new QLabel("Unknown", panel);
    cpu_label->setAlignment(Qt::AlignCenter);
    cpu_label->setWordWrap(true);
    layout->addWidget(cpu_label);

    return {panel, navigation, cpu_label};
}