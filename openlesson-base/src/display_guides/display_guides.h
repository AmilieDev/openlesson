#ifndef DISPLAY_ACPI_GUIDE
#define DISPLAY_ACPI_GUIDE

#include <filesystem>
#include <string>

void display_guide(const std::filesystem::path& docs, const std::string& cpu_brand, const std::string& form_factor, const std::string& guide_type);

#endif