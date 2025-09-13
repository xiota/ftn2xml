// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>

namespace Fountain {
std::string ftn2html(
    const std::string &input,
    const std::string &css_fn = "fountain-html.css",
    const bool &embed_css = false
);
}
