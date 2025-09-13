// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#define FOUNTAIN_WHITESPACE " \t\n\r\f\v"

// Trimming
std::string &ltrim_inplace(std::string &s, const char *t = FOUNTAIN_WHITESPACE);
std::string &rtrim_inplace(std::string &s, const char *t = FOUNTAIN_WHITESPACE);
std::string &trim_inplace(std::string &s, const char *t = FOUNTAIN_WHITESPACE);

// Replace
std::string &replace_all_inplace(
    std::string &subject,
    const std::string_view &search,
    const std::string_view &replace
);
std::string replace_all(
    std::string subject,
    const std::string_view &search,
    const std::string_view &replace
);

// Whitespace trim wrappers
std::string ws_ltrim(std::string s);
std::string ws_rtrim(std::string s);
std::string ws_trim(std::string s);

// String tests
bool begins_with(const std::string &input, const std::string &match);

// Split
std::vector<std::string>
split_string(const std::string_view &str, const std::string_view &delimiter = " ");
std::vector<std::string> split_lines(const std::string_view &s);

// Case conversion
std::string &to_upper_inplace(std::string &s);
std::string &to_lower_inplace(std::string &s);
std::string to_upper(std::string s);
std::string to_lower(std::string s);
bool is_upper(const std::string_view &s);

// HTML entity encoding/decoding
std::string &encode_entities_inplace(std::string &input, bool bProcessAllEntities = false);
std::string encode_entities(std::string input, bool bProcessAllEntities = false);
std::string &decode_entities_inplace(std::string &input);
std::string decode_entities(std::string input);

// C-string helpers
std::string cstr_assign(char *input);
std::vector<std::string> cstrv_assign(char **input);
std::vector<std::string> cstrv_copy(const char *const *input);
std::vector<char *> cstrv_get(const std::vector<std::string> input);

// Regex error printer
void print_regex_error(std::regex_error &e, const char *file, int line);
