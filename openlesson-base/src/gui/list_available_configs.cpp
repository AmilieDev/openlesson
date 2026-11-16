
#include "list_available_configs.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

QVector<HardwareConfig> list_hardware_available(const QString& directory){
    QVector<HardwareConfig> configs;

    const auto docs = std::filesystem::u8path(directory.toUtf8().constData());
    const auto path = docs / "cpu_families.json";

    std::ifstream index(path);
    if(!index){
        std::cerr << "Could not open " << path << '\n';
        return configs;
    }

    try{
        const auto profiles = nlohmann::json::parse(index);

        for(const auto& profile : profiles){
            const auto& match = profile.at("match");
            const auto& brands = match.at("cpuBrandContains");
            const auto preset = profile.at("preset").get<std::string>();

            auto add_config = [&](const std::string& brand){
                if(brand.empty())
                    return;

                HardwareConfig config;
                config.name = QString::fromStdString(brand);
                config.cpu_brand = QString::fromStdString(brand);
                config.preset = QString::fromStdString(preset);

                configs.append(config);
            };

            if(brands.is_string()){
                add_config(brands.get<std::string>());
            }else if(brands.is_array()){
                for(const auto& brand : brands){
                    if(brand.is_string())
                        add_config(brand.get<std::string>());
                }
            }
        }
    }catch(const nlohmann::json::exception& error){
        std::cerr << "Invalid cpu_families.json: " << error.what() << '\n';
    }

    return configs;
}
