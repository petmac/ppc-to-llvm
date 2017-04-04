#pragma once

#include <string>
#include <vector>

std::vector<char> load_binary_file(const char *path);
bool load_text_file(std::string &out, const char *path);
