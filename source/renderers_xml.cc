// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderers_xml.h"

#include <regex>
#include <string>

#include "model_script.h"
#include "parser_fountain.h"
#include "utils_file.h"
#include "utils_string.h"

namespace Fountain {

std::string
ftn2xml(const std::string &input, const std::string &css_fn, const bool &embed_css) {
  std::string output{ "<!DOCTYPE html>\n<html>\n<head>\n" };

  if (!css_fn.empty()) {
    if (embed_css) {
      std::string css_contents = file_get_contents(css_fn);
      output += "<style type='text/css'>\n";
      output += css_contents;
      output += "\n</style>\n";
    } else {
      output += R"(<link rel="stylesheet" type="text/css" href=")";
      output += ((css_fn[0] == '/') ? "file://" : "") + css_fn;
      output += "'>\n";
    }
  }

  output += "</head>\n<body>\n";

  Fountain::Script script;
  script.parseFountain(input);

  output += script.to_string(
      Fountain::ScriptNodeType::ftnContinuation | Fountain::ScriptNodeType::ftnKeyValue |
      Fountain::ScriptNodeType::ftnUnknown
  );

  output += "\n</body>\n</html>\n";

  try {
    static const std::regex re_newlines(R"(\n+)");
    output = std::regex_replace(output, re_newlines, "\n");
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  return output;
}

}  // namespace Fountain
