#include "config_editor.h"

#include "../plist_editor/config_document.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QDomDocument>
#include <QByteArray>
#include <QSignalBlocker>
#include <QStringList>
#include <QVariant>

#include <memory>
#include <vector>

config_editor_widgets build_config_editor(QWidget* parent){
    auto* panel = new QWidget(parent);
    panel->setObjectName("config_editor");

    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    auto* header = new QHBoxLayout;
    auto* title = new QLabel("Config Editor", panel);
    title->setObjectName("config_editor_title");
    header->addWidget(title, 1);
    header->addWidget(new QLabel("config.plist", panel));

    auto* save_button = new QPushButton("Save", panel);
    save_button->setObjectName("config_save");
    save_button->setEnabled(false);
    header->addWidget(save_button);
    layout->addLayout(header);

    auto* status_label = new QLabel("No config loaded", panel);
    status_label->setObjectName("config_status");
    status_label->setWordWrap(true);
    layout->addWidget(status_label);

    auto* tree = new QTreeWidget(panel);
    tree->setObjectName("config_tree");
    tree->setColumnCount(3);
    tree->setHeaderLabels({"Key", "Type", "Value"});
    tree->setAlternatingRowColors(true);
    layout->addWidget(tree, 1);

    auto* footer = new QWidget(panel);
    footer->setObjectName("config_quick_edit");
    auto* footer_layout = new QFormLayout(footer);
    footer_layout->setContentsMargins(0, 6, 0, 0);
    
    auto* quick_select = new QComboBox(footer);
    quick_select->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    quick_select->setMinimumContentsLength(12);

    quick_select->setPlaceholderText("Choose a key…");
    quick_select->setEnabled(false);
    footer_layout->addRow("Quick Select", quick_select);

    auto* quick_value = new QStackedWidget(footer);

    auto* text_value = new QLineEdit(quick_value);
    text_value->setObjectName("quick_text_value");
    text_value->setPlaceholderText("Select a key to edit");
    quick_value->addWidget(text_value);

    auto* bool_value = new QCheckBox("True", quick_value);
    bool_value->setObjectName("quick_bool_value");
    quick_value->addWidget(bool_value);

    quick_value->setEnabled(false);
    footer_layout->addRow("Quick Modify Value", quick_value);
    layout->addWidget(footer);

    return {panel, tree, save_button, status_label, quick_select, quick_value};
}

struct config_value_row{
    QTreeWidgetItem* item;
    QDomElement element;
};

static void add_plist_rows(QTreeWidgetItem& parent, const QDomElement& container, std::vector<config_value_row>& values){
    const bool dictionary = container.tagName() == "dict";
    int index = 0;

    for(auto element = container.firstChildElement();
        !element.isNull();
        element = element.nextSiblingElement()){
        QString key;
        QDomElement value = element;

        if(dictionary){
            if(element.tagName() != "key"){
                break;
            }

            key = element.text();
            value = element.nextSiblingElement();
            if(value.isNull() || value.tagName() == "key"){
                break;
            }

            element = value;
        }else{
            key = QString("[%1]").arg(index++);
        }

        const QString tag = value.tagName();
        auto* row = new QTreeWidgetItem(&parent);
        row->setText(0, key);
        row->setText(1, tag);

        if(tag == "dict" || tag == "array"){
            add_plist_rows(*row, value, values);
        }else if(tag == "true" || tag == "false"){
            row->setText(1, "boolean");
            row->setText(2, tag);
        }else{
            row->setText(2, value.text());
        }
        
        if(tag != "dict" && tag != "array"){
            row->setData(0, Qt::UserRole, static_cast<int>(values.size()));
            values.push_back({row, value});
        }
    }
}

static QString config_row_path(const QTreeWidgetItem* item){
    QStringList parts;

    for(; item != nullptr; item = item->parent()){
        parts.prepend(item->text(0));
    }

    return parts.join(" / ");
}

void load_config_into_editor(const config_editor_widgets& editor, QDomDocument document){
    auto values = std::make_shared<std::vector<config_value_row>>();

    editor.tree->clear();
    editor.quick_select->clear();
    add_plist_rows(*editor.tree->invisibleRootItem(), document.documentElement().firstChildElement(), *values);

    for(const auto& value : *values){
        editor.quick_select->addItem(config_row_path(value.item));
    }

    editor.quick_select->setCurrentIndex(-1);
    editor.quick_select->setEnabled(!values->empty());

    auto* text = editor.quick_value->findChild<QLineEdit*>("quick_text_value");
    auto* checkbox = editor.quick_value->findChild<QCheckBox*>("quick_bool_value");

    QObject::connect(editor.quick_select, &QComboBox::currentIndexChanged, editor.panel, [editor, values, text, checkbox](int index){
        editor.quick_value->setEnabled(index >= 0);
        if(index < 0){
            return;
        }

        const auto& value = values->at(index);
        const bool is_boolean = value.item->text(1) == "boolean";

        const QSignalBlocker block_text(text);
        const QSignalBlocker block_checkbox(checkbox);
        text->setText(value.item->text(2));
        checkbox->setChecked(value.element.tagName() == "true");
        if(is_boolean){
           editor.quick_value->setCurrentWidget(checkbox);
        }else{
           editor.quick_value->setCurrentWidget(text);
        }
        
        editor.tree->setCurrentItem(value.item);
        editor.tree->scrollToItem(value.item);
    });

    QObject::connect(editor.tree, &QTreeWidget::itemDoubleClicked, editor.panel, [editor](QTreeWidgetItem* item, int column){
        const auto index = item->data(0, Qt::UserRole);
        if(column != 2 || !index.isValid()){
            return;
        }

        editor.quick_select->setCurrentIndex(index.toInt());
        editor.quick_value->currentWidget()->setFocus();
    });

    const auto mark_changed = [editor](){
        editor.save_button->setEnabled(true);
        editor.status_label->setText("Unsaved changes");
    };

    QObject::connect(checkbox, &QCheckBox::toggled, editor.panel, [editor, values, mark_changed](bool checked){
        const int index = editor.quick_select->currentIndex();
        if(index < 0){
            return;
        }

        auto& value = values->at(index);
        const QString tag = checked ? "true" : "false";
        if(value.element.tagName() != tag){
            value.element.setTagName(tag);
            value.item->setText(2, tag);
            mark_changed();
        }
    });

    auto commit_text = [editor, values, text, mark_changed, document]() mutable {
        const int index = editor.quick_select->currentIndex();
        if(index < 0 || editor.quick_value->currentWidget() != text){
            return true;
        }

        auto& value = values->at(index);
        const QString tag = value.element.tagName();
        const QString input = text->text();

        if(input == value.item->text(2)){
            return true;
        }

        if(tag == "integer"){
            bool valid = false;
            input.toLongLong(&valid);
            if(!valid){
                editor.status_label->setText("Enter a valid decimal integer.");
                return false;
            }
        }else if(tag == "data"){
            QByteArray base64 = input.toLatin1();
            base64.replace(" ", "").replace("\t", "").replace("\n", "").replace("\r", "");
            const auto decoded = QByteArray::fromBase64Encoding(
                base64, QByteArray::AbortOnBase64DecodingErrors);
            if(!decoded || base64 != decoded.decoded.toBase64()){
                editor.status_label->setText("Enter valid Base64 data.");
                return false;
            }
        }

        while(!value.element.firstChild().isNull()){
            value.element.removeChild(value.element.firstChild());
        }
        value.element.appendChild(document.createTextNode(input));
        value.item->setText(2, input);
        mark_changed();
        return true;
    };

    QObject::connect(text, &QLineEdit::editingFinished, editor.panel, commit_text);

    QObject::connect(editor.save_button, &QPushButton::clicked, editor.panel, [editor, document, commit_text]() mutable {
        if(!commit_text()){
            return;
        }

        QString error;
        if(!save_config_document(document, error)){
            editor.status_label->setText(error);
            return;
        }

        editor.save_button->setEnabled(false);
        editor.status_label->setText("Saved to " + config_output_path());
    });
}