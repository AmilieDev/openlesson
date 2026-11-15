#ifndef OPENLESSON_CONFIG_EDITOR_H
#define OPENLESSON_CONFIG_EDITOR_H

class QWidget;
class QLabel;
class QPushButton;
class QTreeWidget;
class QComboBox;
class QStackedWidget;
class QDomDocument;

struct config_editor_widgets{
    QWidget* panel;
    QTreeWidget* tree;
    QPushButton* save_button;
    QLabel* status_label;
    QComboBox* quick_select;
    QStackedWidget* quick_value;
};

config_editor_widgets build_config_editor(QWidget* parent);
void load_config_into_editor(const config_editor_widgets& editor, QDomDocument document);

#endif