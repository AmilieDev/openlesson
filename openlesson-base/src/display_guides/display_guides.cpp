#include "display_guides.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>

std::string find_guide_preset(const std::filesystem::path& docs, const std::string& cpu_brand, const std::string& form_factor){
    if(cpu_brand.empty()){
        return {};
    }

    const auto path = docs / "cpu_families.json";
    std::ifstream index(path);
    if(!index){
        std::cerr << "Could not open " << path << '\n';
        return {};
    }

    try{
        const auto profiles = nlohmann::json::parse(index);

        for(const auto& profile : profiles){
            const auto& match = profile.at("match");
            const auto& formFactors = match.at("formFactor");

            bool formFactorMatches = false;

            if (formFactors.is_string()) {
                formFactorMatches =
                    formFactors.get<std::string>() == form_factor;
            } else if (formFactors.is_array()) {
                formFactorMatches = std::any_of(
                    formFactors.begin(),
                    formFactors.end(),
                    [&](const auto& factor) {
                        return factor.template get<std::string>() == form_factor;
                    }
                );
            }

        if (!formFactorMatches) {
            continue;
        }

            for(const auto& entry : match.at("cpuBrandContains")){
                const auto pattern = entry.get<std::string>();
                if(!pattern.empty() && cpu_brand.find(pattern) != std::string::npos){
                    return profile.at("preset").get<std::string>();
                }
            }
        }
    }catch(const nlohmann::json::exception& error){
        std::cerr << "Invalid cpu_families.json: " << error.what() << '\n';
    }

    return {};
}