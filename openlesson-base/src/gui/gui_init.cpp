#include "gui_init.h"

#include "../plist_editor/config_document.h"
#include "config_editor.h"
#include "guide_panel.h"
#include "sidebar.h"
#include "../display_guides/display_guides.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QString>
#include <QWidget>
#include <QDir>
#include <QFileInfo>
#include <QTreeWidget>

void build_main_window(QMainWindow& window, const std::string& cpu_brand){
    const char* docs_path = "openlesson-base/docs";
    const QString docs_dir = QString::fromUtf8(docs_path);

    auto* root = new QWidget(&window);
    auto* layout = new QHBoxLayout(root);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    const auto sidebar = build_sidebar(root);
    layout->addWidget(sidebar.panel);

    auto* splitter = new QSplitter(Qt::Horizontal, root);
    splitter->setChildrenCollapsible(false);
    layout->addWidget(splitter, 1);

    const auto guide = build_guide_panel(splitter, docs_dir);
    const auto editor = build_config_editor(splitter);
    
    const QString default_path = QDir(docs_dir).filePath("default.plist");
    const bool has_saved_config = QFileInfo::exists(config_output_path());

    QDomDocument document;
    QString config_error;

    if(load_config_document(default_path, document, config_error)){
        load_config_into_editor(editor, document);
        editor.status_label->setText(
            has_saved_config
                ? QString("Opened %1").arg(config_output_path())
                : QString("Loaded default.plist"));
    }else{
        editor.status_label->setWordWrap(true);
        editor.status_label->setText(config_error);
    }

    splitter->addWidget(guide.pages);
    splitter->addWidget(editor.panel);
    splitter->setSizes({440, 485});

    QObject::connect(sidebar.navigation, &QListWidget::currentRowChanged, guide.pages, &QStackedWidget::setCurrentIndex);

    if(!cpu_brand.empty()){
        sidebar.cpu_label->setText(QString::fromStdString(cpu_brand).trimmed());
    }

    const auto preset = find_guide_preset(docs_path, cpu_brand, "desktop");
    load_efi_guide(*guide.efi_guide, docs_dir, QString::fromStdString(preset));

    sidebar.navigation->setCurrentRow(1);

    QFile stylesheet("openlesson-base/src/gui/everforest.qss");
    if(stylesheet.open(QIODevice::ReadOnly)){
        window.setStyleSheet(QString::fromUtf8(stylesheet.readAll()));
    }

    window.setCentralWidget(root);
    window.setWindowTitle("OpenLesson");
    window.resize(1100, 680);
}