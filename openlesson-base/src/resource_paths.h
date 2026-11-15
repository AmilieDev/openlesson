#ifndef OPENLESSON_RESOURCE_PATHS_H
#define OPENLESSON_RESOURCE_PATHS_H

#include <QCoreApplication>
#include <QDir>
#include <QString>

inline QDir resource_root(){
    const QDir executable_dir(QCoreApplication::applicationDirPath());

    #if defined(__APPLE__)
        const QDir packaged(executable_dir.filePath("../Resources"));
    #else
        const QDir packaged(executable_dir.filePath("../share/openlesson"));
    #endif

    if(packaged.exists("docs/default.plist")){
        return packaged;
    }

    return QDir(QDir::current().absoluteFilePath("openlesson-base"));
}

#endif