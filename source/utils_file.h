// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::string file_get_contents(const std::string &filename);
bool file_set_contents(const std::string &filename, const std::string &contents);

std::vector<std::uint8_t> file_get_data(const std::string &filename);
bool file_set_data(const std::string &filename, const std::vector<std::uint8_t> &contents);
