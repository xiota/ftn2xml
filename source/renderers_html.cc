// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderers_html.h"

#include <regex>
#include <string>

#include "model_script.h"
#include "parser_fountain.h"
#include "utils_file.h"
#include "utils_string.h"

namespace Fountain {

std::string
ftn2html(const std::string &input, const std::string &css_fn, const bool &embed_css) {
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
    replace_all_inplace(output, "<Fountain>", R"(<div class="Fountain">)");
    replace_all_inplace(output, "</Fountain>", "</div>");

    replace_all_inplace(output, "<Transition>", R"(<div class="Transition">)");
    replace_all_inplace(output, "</Transition>", "</div>");

    replace_all_inplace(output, "<SceneHeader>", R"(<div class="SceneHeader">)");
    replace_all_inplace(output, "</SceneHeader>", "</div>");

    replace_all_inplace(output, "<Action>", R"(<div class="Action">)");
    replace_all_inplace(output, "</Action>", "</div>");

    replace_all_inplace(output, "<Lyric>", R"(<div class="Lyric">)");
    replace_all_inplace(output, "</Lyric>", "</div>");

    replace_all_inplace(output, "<Character>", R"(<div class="Character">)");
    replace_all_inplace(output, "</Character>", "</div>");

    replace_all_inplace(output, "<Parenthetical>", R"(<div class="Parenthetical">)");
    replace_all_inplace(output, "</Parenthetical>", "</div>");

    replace_all_inplace(output, "<Speech>", R"(<div class="Speech">)");
    replace_all_inplace(output, "</Speech>", "</div>");

    replace_all_inplace(output, "<Dialog>", R"(<div class="Dialog">)");
    replace_all_inplace(output, "</Dialog>", "</div>");

    replace_all_inplace(output, "<DialogDual>", R"(<div class="DialogDual">)");
    replace_all_inplace(output, "</DialogDual>", "</div>");

    replace_all_inplace(output, "<DialogLeft>", R"(<div class="DialogLeft">)");
    replace_all_inplace(output, "</DialogLeft>", "</div>");

    replace_all_inplace(output, "<DialogRight>", R"(<div class="DialogRight">)");
    replace_all_inplace(output, "</DialogRight>", "</div>");

    replace_all_inplace(output, "<PageBreak>", R"(<div class="PageBreak">)");
    replace_all_inplace(output, "</PageBreak>", "</div>");

    replace_all_inplace(output, "<Note>", R"(<div class="Note">)");
    replace_all_inplace(output, "</Note>", "</div>");

    replace_all_inplace(output, "<ActionCenter>", R"(<center>)");
    replace_all_inplace(output, "</ActionCenter>", "</center>");

    replace_all_inplace(output, "<BlankLine>", "");
    replace_all_inplace(output, "</BlankLine>", "");

    for (std::size_t i = 1; i <= 6; i++) {
      std::string lvl = std::to_string(i);
      replace_all_inplace(
          output, "<SectionH" + lvl + ">", R"(<div class="SectionH)" + lvl + R"(">)"
      );
      replace_all_inplace(output, "</SectionH" + lvl + ">", "</div>");

      replace_all_inplace(
          output, "<SynopsisH" + lvl + ">", R"(<div class="SynopsisH)" + lvl + R"(">)"
      );
      replace_all_inplace(output, "</SynopsisH" + lvl + ">", "</div>");
    }

    static const std::regex re_newlines(R"(\n+)");
    output = std::regex_replace(output, re_newlines, "\n");
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  return output;
}

}  // namespace Fountain
