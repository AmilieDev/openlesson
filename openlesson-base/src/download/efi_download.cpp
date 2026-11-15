#include "efi_download.h"

#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QTextCursor>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

QWidget* build_efi_download(QWidget* parent){
    auto* panel = new QWidget(parent);
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* button = new QPushButton("Prepare OpenCore EFI", panel);

    auto* status = new QLabel(panel);
    status->setTextFormat(Qt::PlainText);
    status->setWordWrap(true);

    auto* log = new QPlainTextEdit(panel);
    log->setReadOnly(true);
    log->setMaximumBlockCount(500);
    log->hide();

    layout->addWidget(button);
    layout->addWidget(status);
    layout->addWidget(log);

    auto* process = new QProcess(panel);
    process->setProcessChannelMode(QProcess::MergedChannels);

    auto* hide_timer = new QTimer(panel);
    hide_timer->setSingleShot(true);
    hide_timer->setInterval(5000);

    QObject::connect(hide_timer, &QTimer::timeout, panel, &QWidget::hide);

    const auto read_log = [process, log](){
        log->moveCursor(QTextCursor::End);
        log->insertPlainText(QString::fromUtf8(process->readAllStandardOutput()));
        log->ensureCursorVisible();
    };

    QObject::connect(process, &QProcess::readyReadStandardOutput, panel, read_log);

    const QString output = QDir::home().filePath("OLB/EFI");

    QObject::connect(button, &QPushButton::clicked, panel, [process, button, status, log, output](){
        const QFileInfo script("openlesson-base/tools/prepare_efi/prepare_efi.py");
        if(!script.isFile()){
            status->setText("Could not find " + script.absoluteFilePath());
            return;
        }

        QStringList args;
        
        #if defined(_WIN32)
                const QString python = "py";
                args << "-3";
        #else
                const QString python = "python3";
        #endif
                args << "-u" << script.absoluteFilePath() << output;

        log->clear();
        log->show();
        button->setEnabled(false);
        status->setText("Preparing OpenCore EFI…");

        process->start(python, args);
    });

    QObject::connect(process, &QProcess::errorOccurred, panel,
                     [process, button, status](QProcess::ProcessError error){
        if(error == QProcess::FailedToStart){
            status->setText("Could not start Python 3: " + process->errorString());
            button->setEnabled(true);
        }
    });

    QObject::connect(process, &QProcess::finished, panel,
                     [button, status, output, read_log, hide_timer]
                     (int code, QProcess::ExitStatus exit_status){
        read_log();

        if(exit_status == QProcess::NormalExit && code == 0){
            status->setText("EFI ready at " + QDir::toNativeSeparators(output));
            hide_timer->start();
        }else{
            status->setText("Could not prepare the EFI. See the log.");
            button->setEnabled(true);
        }
    });

    return panel;
}