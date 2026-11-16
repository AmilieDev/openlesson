
#pragma once

#include <QString>
#include <QVector>

struct HardwareConfig {
    QString name;
    QString cpu_brand;
    QString preset;
};

QVector<HardwareConfig> list_hardware_available(const QString& directory);
