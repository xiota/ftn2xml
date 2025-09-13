// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "model_script.h"
#include <string>

namespace Fountain {

// Parse a Fountain-formatted screenplay into the given Script object.
void parseFountain(Script &script, const std::string &text);

}  // namespace Fountain
