// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utils_file.h"

#include <fstream>
#include <iterator>

std::string file_get_contents(const std::string &filename) {
  try {
    std::ifstream instream(filename, std::ios::in);
    std::string contents(
        (std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>())
    );
    instream.close();
    return contents;
  } catch (...) {
    return {};
  }
}

bool file_set_contents(const std::string &filename, const std::string &contents) {
  try {
    std::ofstream outstream(filename, std::ios::out);
    std::copy(contents.begin(), contents.end(), std::ostream_iterator<char>(outstream));
    outstream.close();
    return true;
  } catch (...) {
    return false;
  }
}

std::vector<std::uint8_t> file_get_data(const std::string &filename) {
  try {
    std::ifstream instream(filename, std::ios::in | std::ios::binary);
    std::vector<std::uint8_t> contents(
        (std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>())
    );
    instream.close();
    return contents;
  } catch (...) {
    return {};
  }
}

bool file_set_data(const std::string &filename, const std::vector<std::uint8_t> &contents) {
  try {
    std::ofstream outstream(filename, std::ios::out | std::ios::binary);
    std::copy(contents.begin(), contents.end(), std::ostream_iterator<char>(outstream));
    outstream.close();
    return true;
  } catch (...) {
    return false;
  }
}
