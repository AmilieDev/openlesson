
#include "gui_init.h"

#include "../display_guides/display_guides.h"
#include "../plist_editor/config_document.h"
#include "../resource_paths.h"
#include "config_editor.h"
#include "guide_panel.h"
#include "sidebar.h"
#include "list_available_configs.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QString>
#include <QWidget>

#include <filesystem>

void build_main_window(QMainWindow& window, std::string& cpu_brand){
    const QDir resources = resource_root();
    const QDir docs(resources.filePath("docs"));
    const QString docs_dir = docs.absolutePath();
    const auto docs_path = std::filesystem::u8path(docs_dir.toUtf8().constData());

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

    const QString default_path = docs.filePath("default.plist");
    const bool has_saved_config = QFileInfo::exists(config_output_path());

    QDomDocument document;
    QString config_error;

    if(load_config_document(default_path, document, config_error)){
        load_config_into_editor(editor, document);
        editor.status_label->setText(has_saved_config ? QString("Opened %1").arg(config_output_path()) : QString("Loaded default.plist"));
    }else{
        editor.status_label->setWordWrap(true);
        editor.status_label->setText(config_error);
    }

    splitter->addWidget(guide.pages);
    splitter->addWidget(editor.panel);
    splitter->setSizes({440, 485});

    auto* hardware_page = new QWidget(guide.pages);
    auto* hardware_layout = new QVBoxLayout(hardware_page);
    hardware_layout->setContentsMargins(20, 20, 20, 20);
    hardware_layout->setSpacing(12);

    auto* hardware_title = new QLabel("Hardware Library", hardware_page);
    hardware_layout->addWidget(hardware_title);

    auto* hardware_description = new QLabel("Select a hardware configuration to use when preparing your EFI.", hardware_page);
    hardware_description->setWordWrap(true);
    hardware_layout->addWidget(hardware_description);

    auto* hardware_list = new QListWidget(hardware_page);
    hardware_list->setObjectName("hardware_library_list");
    hardware_layout->addWidget(hardware_list, 1);

    auto* active_label = new QLabel(hardware_page);
    active_label->setWordWrap(true);
    hardware_layout->addWidget(active_label);

    auto* select_button = new QPushButton("Use Selected Hardware", hardware_page);
    hardware_layout->addWidget(select_button);

    guide.pages->addWidget(hardware_page);

    const auto configs = list_hardware_available(docs_dir);

    for(const auto& config : configs){
        auto* item = new QListWidgetItem(config.name, hardware_list);
        item->setData(Qt::UserRole, config.cpu_brand);
        item->setToolTip(QString("CPU: %1\nPreset: %2").arg(config.cpu_brand, config.preset));
    }

    if(configs.isEmpty()){
        auto* item = new QListWidgetItem("No hardware configurations found", hardware_list);
        item->setFlags(Qt::NoItemFlags);
        select_button->setEnabled(false);
    }

    auto reload_guide = [
        &cpu_brand,
        cpu_label = sidebar.cpu_label,
        active_label,
        efi_guide = guide.efi_guide,
        docs_path,
        docs_dir
    ](){
        const QString brand = QString::fromStdString(cpu_brand).trimmed();

        cpu_label->setText(brand.isEmpty() ? "Unknown" : brand);
        active_label->setText("Active CPU: " + (brand.isEmpty() ? "Unknown" : brand));

        const auto preset = find_guide_preset(docs_path, cpu_brand, "desktop");
        load_efi_guide(*efi_guide, docs_dir, QString::fromStdString(preset));
    };

    auto select_hardware = [
        &cpu_brand,
        hardware_list,
        navigation = sidebar.navigation,
        reload_guide
    ](){
        auto* item = hardware_list->currentItem();

        if(!item)
            return;

        const QString selected_brand = item->data(Qt::UserRole).toString();

        if(selected_brand.isEmpty())
            return;

        cpu_brand = selected_brand.toStdString();

        reload_guide();
        navigation->setCurrentRow(1);
    };

    QObject::connect(select_button, &QPushButton::clicked, hardware_page, select_hardware);

    QObject::connect(hardware_list, &QListWidget::itemDoubleClicked, hardware_page, [select_hardware](QListWidgetItem*){
        select_hardware();
    });

    QObject::connect(sidebar.navigation, &QListWidget::currentRowChanged, guide.pages, &QStackedWidget::setCurrentIndex);

    reload_guide();
    sidebar.navigation->setCurrentRow(1);

    QFile stylesheet(resources.filePath("everforest.qss"));
    if(stylesheet.open(QIODevice::ReadOnly)){
        window.setStyleSheet(QString::fromUtf8(stylesheet.readAll()));
    }

    window.setCentralWidget(root);
    window.setWindowTitle("OpenLesson");
    window.resize(1100, 680);
}
