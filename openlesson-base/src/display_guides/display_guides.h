#ifndef DISPLAY_ACPI_GUIDE_H
#define DISPLAY_ACPI_GUIDE_H

#include <filesystem>
#include <string>

std::string find_guide_preset(const std::filesystem::path& docs, const std::string& cpu_brand, const std::string& form_factor);

#endif