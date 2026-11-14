#include "display_guides.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>

void display_guide(const std::filesystem::path& docs, const std::string& cpu_brand, const std::string& form_factor, const std::string& guide_type){
    std::ifstream index(docs / "cpu_families.json");
    if(!index){
        std::cerr << "Couldn't open cpu_families.json. Ensure you have docs with your build of the program please.";
        return;
    }
    
    try {
        const auto profiles = nlohmann::json::parse(index);
        std::string filename;

        for (const auto& profile : profiles) {
            const auto& match = profile.at("match");
            if (match.at("formFactor") != form_factor) {
                continue;
            }

            const auto& patterns = match.at("cpuBrandContains");
            const bool cpu_matches = std::any_of(
                patterns.begin(), patterns.end(), [&](const auto& entry) {
                    const auto pattern = entry.template get<std::string>();
                    return !pattern.empty() && cpu_brand.find(pattern) != std::string::npos;
                });

            if (!cpu_matches) {
                continue;
            }
            
            const auto preset = profile.at("preset").get<std::string>();
            
            if (guide_type == "acpi") {
            filename = "acpi.md";
            } else if (guide_type == "kernquirk") {
                filename = "kernel_quirks.md";
            } else if (guide_type == "bootquirk") {
                filename = "booter_quirks.md";
            } else {
                std::cerr << "Unknown guide type: " << guide_type << '\n';
                return;
            }

            const auto path = docs / "known_guides" / preset / filename;
            std::ifstream guide(path);

            if (!guide) {
                std::cerr << "Could not open " << path << '\n';
                return;
            }

            std::cout << guide.rdbuf() << '\n';
            return;
        }

        std::cout << "No guide matches this hardware.\n";
    } catch (const nlohmann::json::exception& error) {
        std::cerr << "Invalid cpu_families.json: " << error.what() << '\n';
    }
}