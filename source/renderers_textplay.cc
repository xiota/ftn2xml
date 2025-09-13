// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderers_textplay.h"

#include <regex>
#include <string>

#include "model_script.h"
#include "parser_fountain.h"
#include "utils_file.h"
#include "utils_string.h"

namespace Fountain {

std::string
ftn2textplay(const std::string &input, const std::string &css_fn, const bool &embed_css) {
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

  output +=
      "</head>\n<body>\n"
      "<div id=\"wrapper\" class=\"fountain\">\n";

  Fountain::Script script;
  script.parseFountain(input);

  output += script.to_string(
      Fountain::ScriptNodeType::ftnContinuation | Fountain::ScriptNodeType::ftnKeyValue |
      Fountain::ScriptNodeType::ftnUnknown
  );

  output += "\n</div>\n</body>\n</html>\n";

  try {
    replace_all_inplace(output, "<Transition>", R"(<h3 class="right-transition">)");
    replace_all_inplace(output, "</Transition>", "</h3>");

    replace_all_inplace(output, "<SceneHeader>", R"(<h2 class="full-slugline">)");
    replace_all_inplace(output, "</SceneHeader>", "</h2>");

    replace_all_inplace(output, "<Action>", R"(<p class="action">)");
    replace_all_inplace(output, "</Action>", "</p>");

    replace_all_inplace(output, "<Lyric>", R"(<span class="lyric">)");
    replace_all_inplace(output, "</Lyric>", "</span>");

    replace_all_inplace(output, "<Character>", R"(<dt class="character">)");
    replace_all_inplace(output, "</Character>", "</dt>");

    replace_all_inplace(output, "<Parenthetical>", R"(<dd class="parenthetical">)");
    replace_all_inplace(output, "</Parenthetical>", "</dd>");

    replace_all_inplace(output, "<Speech>", R"(<dd class="dialogue">)");
    replace_all_inplace(output, "</Speech>", "</dd>");

    replace_all_inplace(output, "<Dialog>", R"(<div class="dialog">)");
    replace_all_inplace(output, "</Dialog>", "</div>");

    replace_all_inplace(output, "<DialogDual>", R"(<div class="dialog_wrapper">)");
    replace_all_inplace(output, "</DialogDual>", "</div>");

    replace_all_inplace(output, "<DialogLeft>", R"(<dl class="first">)");
    replace_all_inplace(output, "</DialogLeft>", "</dl>");

    replace_all_inplace(output, "<DialogRight>", R"(<dl class="second">)");
    replace_all_inplace(output, "</DialogRight>", "</dl>");

    replace_all_inplace(output, "</PageBreak>", R"(<div class="page-break">)");
    replace_all_inplace(output, "</PageBreak>", "</div>");

    replace_all_inplace(output, "<Note>", R"(<p class="comment">)");
    replace_all_inplace(output, "</Note>", "</p>");

    replace_all_inplace(output, "<BlankLine>", "");
    replace_all_inplace(output, "</BlankLine>", "");

    replace_all_inplace(output, "<ActionCenter>", R"(<p class="center">)");
    replace_all_inplace(output, "</ActionCenter>", "</p>");

    static const std::regex re_newlines(R"(\n+)");
    output = std::regex_replace(output, re_newlines, "\n");
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  return output;
}

}  // namespace Fountain
