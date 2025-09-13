// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>

namespace Fountain {
std::string ftn2textplay(
    const std::string &input,
    const std::string &css_fn = "textplay.css",
    const bool &embed_css = false
);
}
