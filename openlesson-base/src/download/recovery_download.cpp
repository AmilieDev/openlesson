#include "recovery_download.h"

#include "../resource_paths.h"

#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QTextCursor>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

QWidget* build_recovery_download(QWidget* parent){
    auto* panel = new QWidget(parent);
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* button = new QPushButton("Download Tahoe Recovery", panel);
    auto* status = new QLabel(panel);
    status->setWordWrap(true);

    auto* log = new QPlainTextEdit(panel);
    log->setReadOnly(true);
    log->setMaximumBlockCount(500);

    layout->addWidget(button);
    layout->addWidget(status);
    layout->addWidget(log);

    auto* process = new QProcess(panel);
    auto* hide_timer = new QTimer(panel);
    hide_timer->setSingleShot(true);
    hide_timer->setInterval(5000);

QObject::connect(hide_timer, &QTimer::timeout, panel, &QWidget::hide);
    process->setProcessChannelMode(QProcess::MergedChannels);

    const auto read_log = [process, log](){
        log->moveCursor(QTextCursor::End);
        log->insertPlainText(QString::fromUtf8(process->readAllStandardOutput()));
        log->ensureCursorVisible();
    };

    QObject::connect(process, &QProcess::readyReadStandardOutput, panel, read_log);

    const QString output = QDir::home().filePath("OLB/com.apple.recovery.boot");

    QObject::connect(button, &QPushButton::clicked, panel, [process, button, status, log, output](){
        const QFileInfo script(resource_root().filePath("tools/macrecovery/macrecovery.py"));

        if(!script.isFile()){
            status->setText("Could not find " + script.absoluteFilePath());
            return;
        }

        if(!QDir().mkpath(output)){
            status->setText("Could not create " + QDir::toNativeSeparators(output));
            return;
        }

        QStringList args;
        
        #if defined(_WIN32)
                const QString python = "py";
                args << "-3";
        #else
                const QString python = "python3";
        #endif

        args << "-u" << script.absoluteFilePath()
             << "-b" << "Mac-CFF7D910A743CAAF"
             << "-m" << "00000000000000000"
             << "-os" << "latest"
             << "-o" << output
             << "download";

        log->clear();
        button->setEnabled(false);
        status->setText("Downloading recovery…");
        process->start(python, args);
    });

    QObject::connect(process, &QProcess::errorOccurred, panel, [process, button, status, hide_timer, panel](QProcess::ProcessError error){
        if(error == QProcess::FailedToStart){
            status->setText("Could not start Python 3: " + process->errorString());
            hide_timer->stop();
            panel->show();
            button->setEnabled(true);
        }
    });

    QObject::connect(process, &QProcess::finished, panel, [button, status, output, read_log, hide_timer, panel](int code, QProcess::ExitStatus exit){
        read_log();
        button->setEnabled(true);

        if(exit == QProcess::NormalExit && code == 0){
            status->setText("Recovery saved to " + QDir::toNativeSeparators(output));
            hide_timer->start();
        }else{
            hide_timer->stop();
            status->setText("Recovery download failed. See the log.");
            panel->show();
        }
    });

    return panel;
}