#pragma once

#include <string>
#include <vector>

extern const std::vector<const char *> data_names;

std::string build_path(const char *name, const char *suffix);
std::string build_path(const char *name, size_t arch_bits, const char *suffix);
std::string build_path(const char *name, size_t arch_bits, size_t address_bits, const char *suffix);
