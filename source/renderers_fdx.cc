// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "renderers_fdx.h"

#include <regex>
#include <string>

#include "model_script.h"
#include "parser_fountain.h"
#include "utils_string.h"

namespace Fountain {

std::string ftn2fdx(const std::string &input) {
  std::string output{ R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>)" };
  output += '\n';
  output += R"(<FinalDraft DocumentType="Script" Template="No" Version="1">)";
  output += "\n<Content>\n";

  Fountain::Script script;
  script.parseFountain(input);

  output += script.to_string(
      Fountain::ScriptNodeType::ftnContinuation | Fountain::ScriptNodeType::ftnKeyValue |
      Fountain::ScriptNodeType::ftnSection | Fountain::ScriptNodeType::ftnSynopsis |
      Fountain::ScriptNodeType::ftnUnknown
  );

  output += "\n</Content>\n</FinalDraft>\n";

  try {
    replace_all_inplace(output, "<Transition>", R"(<Paragraph Type="Transition"><Text>)");
    replace_all_inplace(output, "</Transition>", "</Text></Paragraph>");

    replace_all_inplace(output, "<SceneHeader>", R"(<Paragraph Type="Scene Heading"><Text>)");
    replace_all_inplace(output, "</SceneHeader>", "</Text></Paragraph>");

    replace_all_inplace(output, "<Action>", R"(<Paragraph Type="Action"><Text>)");
    replace_all_inplace(output, "</Action>", "</Text></Paragraph>");

    replace_all_inplace(output, "<Character>", R"(<Paragraph Type="Character"><Text>)");
    replace_all_inplace(output, "</Character>", "</Text></Paragraph>");

    replace_all_inplace(output, "<Parenthetical>", R"(<Paragraph Type="Parenthetical"><Text>)");
    replace_all_inplace(output, "</Parenthetical>", "</Text></Paragraph>");

    replace_all_inplace(output, "<Speech>", R"(<Paragraph Type="Dialogue"><Text>)");
    replace_all_inplace(output, "</Speech>", "</Text></Paragraph>");

    replace_all_inplace(output, "<DualDialog>", "<Paragraph><DualDialog>");
    replace_all_inplace(output, "</DualDialog>", "</DualDialog></Paragraph>");

    replace_all_inplace(
        output, "<ActionCenter>", R"(<Paragraph Type="Action" Alignment="Center"><Text>)"
    );
    replace_all_inplace(output, "</ActionCenter>", "</Text></Paragraph>");

    replace_all_inplace(output, "<b>", R"(<Text Style="Bold">)");
    replace_all_inplace(output, "</b>", "</Text>");

    replace_all_inplace(output, "<i>", R"(<Text Style="Italic">)");
    replace_all_inplace(output, "</i>", "</Text>");

    replace_all_inplace(output, "<u>", R"(<Text Style="Underline">)");
    replace_all_inplace(output, "</u>", "</Text>");

    replace_all_inplace(
        output, "<PageBreak>", R"(<Paragraph Type="Action" StartsNewPage="Yes"><Text>)"
    );
    replace_all_inplace(output, "</PageBreak>", "</Text></Paragraph>");

    replace_all_inplace(output, "<Note>", "<ScriptNote><Text>");
    replace_all_inplace(output, "</Note>", "</Text></ScriptNote>");

    static const std::regex re_dialog("</?Dialog(Left|Right)?>");
    output = std::regex_replace(output, re_dialog, "");

    replace_all_inplace(output, "<BlankLine>", "");
    replace_all_inplace(output, "</BlankLine>", "");

    // Don't know if these work...
    replace_all_inplace(output, "<Lyric>", R"(<Paragraph Type="Lyric"><Text>)");
    replace_all_inplace(output, "</Lyric>", "</Text></Paragraph>");

    static const std::regex re_newlines(R"(\n+)");
    output = std::regex_replace(output, re_newlines, "\n");
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  return output;
}

}  // namespace Fountain
