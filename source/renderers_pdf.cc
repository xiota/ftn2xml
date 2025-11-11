// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderers_pdf.h"
#include "config.h"

#ifdef HAVE_PODOFO

#  include <algorithm>
#  include <cstddef>
#  include <map>
#  include <string>
#  include <tuple>
#  include <vector>

#  include <podofo/podofo.h>

#  include "model_script.h"
#  include "parser_fountain.h"
#  include "utils_string.h"

#  if (PODOFO_VERSION_MINOR < 10) && (PODOFO_VERSION_MAJOR < 1)
#    include "renderers_pdf_0.9.inc"
#  else
#    include "renderers_pdf_0.10.inc"
#  endif

#endif  // HAVE_PODOFO
