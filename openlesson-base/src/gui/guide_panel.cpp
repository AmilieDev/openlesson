#include "guide_panel.h"

#include "../download/efi_download.h"
#include "../download/recovery_download.h"

#include <QDir>
#include <QFile>
#include <QStackedWidget>
#include <QStringList>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>

static bool read_markdown(const QString& path, QString& markdown){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    markdown = QString::fromUtf8(file.readAll());
    return file.error() == QFileDevice::NoError;
}

static void load_markdown_page(QTextBrowser& page, const QString& path){
    QString markdown;

    if(read_markdown(path, markdown)){
        page.setMarkdown(markdown);
    }else{
        page.setPlainText("Could not read guide:\n" + path);
    }
}

static void add_guide_page(QStackedWidget& pages, const QString& path){
    auto* page = new QTextBrowser(&pages);
    load_markdown_page(*page, path);
    pages.addWidget(page);
}

guide_panel_widgets build_guide_panel(QWidget* parent, const QString& docs_dir){
    auto* pages = new QStackedWidget(parent);
    pages->setObjectName("guide_panel");

    const QDir docs(docs_dir);

    auto* download_page = new QWidget(pages);
    auto* download_layout = new QVBoxLayout(download_page);

    auto* instructions = new QTextBrowser(download_page);
    load_markdown_page(*instructions, docs.filePath("guides/downloading_macos.md"));

    download_layout->addWidget(instructions, 1);
    download_layout->addWidget(build_recovery_download(download_page));
    pages->addWidget(download_page);

    auto* efi_page = new QWidget(pages);
    auto* efi_layout = new QVBoxLayout(efi_page);

    auto* efi_guide = new QTextBrowser(efi_page);
    efi_guide->setPlainText("Detecting hardware…");

    efi_layout->addWidget(efi_guide, 1);
    efi_layout->addWidget(build_efi_download(efi_page));
    pages->addWidget(efi_page);

    add_guide_page(*pages, docs.filePath("guides/preparing_to_reboot.md"));
    add_guide_page(*pages, docs.filePath("guides/troubleshooting.md"));

    return {pages, efi_guide};
}

void load_efi_guide(QTextBrowser& page, const QString& docs_dir, const QString& preset){
    if(preset.isEmpty()){
        page.setPlainText("No EFI guide matches this hardware.");
        return;
    }

    const QDir docs(docs_dir);
    const QString guide_dir = "known_guides/" + preset + "/";
    const QStringList filenames = {
        "acpi.md",
        "gathering_kexts.md",
        "booter_quirks.md",
        "kernel_quirks.md"
    };

    QStringList sections;
    for(const auto& filename : filenames){
        QString markdown;
        if(read_markdown(docs.filePath(guide_dir + filename), markdown)){
            sections.append(markdown);
        }
    }

    if(sections.isEmpty()){
        page.setPlainText("Could not read any EFI guides for " + preset + ".");
        return;
    }

    page.setMarkdown(sections.join("\n\n---\n\n"));
}