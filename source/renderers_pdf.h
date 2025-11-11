// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>

namespace Fountain {

// Generate a PDF from Fountain input and write it to fn.
// Returns true on success, false on failure.
// Only compiled if HAVE_PODOFO is defined.
bool ftn2pdf(const std::string &fn, const std::string &input);

}  // namespace Fountain
