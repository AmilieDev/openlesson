#ifndef OPENLESSON_CONFIG_DOCUMENT_H
#define OPENLESSON_CONFIG_DOCUMENT_H

#include <QDomDocument>
#include <QString>

QString config_output_path();

bool load_config_document(const QString& default_path, QDomDocument& document, QString& error);

bool save_config_document(const QDomDocument&, QString& error);

#endif