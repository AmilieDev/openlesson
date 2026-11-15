#ifndef OPENLESSON_GUIDE_PANEL_H
#define OPENLESSON_GUIDE_PANEL_H

#include <QString>

class QWidget;
class QStackedWidget;
class QTextBrowser;

struct guide_panel_widgets{
    QStackedWidget* pages;
    QTextBrowser* efi_guide;
};

guide_panel_widgets build_guide_panel(QWidget* parent, const QString& docs_dir);

void load_efi_guide(QTextBrowser& page, const QString& docs_dir, const QString& preset);

#endif