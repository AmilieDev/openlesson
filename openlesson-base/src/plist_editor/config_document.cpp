#include "config_document.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

QString config_output_path(){
    return QDir::home().filePath("OLB/config.plist");
}

bool load_config_document(const QString& default_path, QDomDocument& document, QString& error){
    const QString output_path = config_output_path();
    const QString source_path = QFileInfo::exists(output_path) ? output_path : default_path;

    QFile file(source_path);
    if(!file.open(QIODevice::ReadOnly)){
        error = source_path + "\n" + file.errorString();
        return false;
    }

    QDomDocument loaded;
    const auto result = loaded.setContent(&file);
    if(!result){
        error = QString("%1, line %2\n%3").arg(source_path).arg(result.errorLine).arg(result.errorMessage);
        return false;
    }

    const auto root = loaded.documentElement();
    if(root.tagName() != "plist" || root.firstChildElement().tagName() != "dict"){
        error = "Expected an OpenCore plist dictionary in " + source_path;
        return false;
    }

    document = loaded;
    error.clear();
    return true;
}

bool save_config_document(const QDomDocument& document, QString& error){
    const QString path = config_output_path();
    const QString directory = QFileInfo(path).absolutePath();

    if(!QDir().mkpath(directory)){
        error = "Could not create " + directory;
        return false;
    }

    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        error = path + "\n" + file.errorString();
        return false;
    }

    const QByteArray xml = document.toByteArray(4);
    if(file.write(xml) != xml.size() || !file.commit()){
        error = path + "\n" + file.errorString();
        return false;
    }

    error.clear();
    return true;
}