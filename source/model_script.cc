// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model_script.h"

#include <regex>

#include "utils_string.h"

namespace Fountain {

std::string ScriptNode::to_string(const int &flags) const {
  static int dialog_state = 0;
  std::string output;

  switch (type) {
    case ScriptNodeType::ftnKeyValue:
      if (flags & type) {
        break;
      }
      output = "<meta>\n<key>" + key + "</key>\n<value>" + value + "</value>\n</meta>\n";
      break;
    case ScriptNodeType::ftnPageBreak:
      if (flags & type) {
        break;
      }
      if (dialog_state) {
        dialog_state == 1   ? output = "</Dialog>\n"
        : dialog_state == 2 ? output = "</DialogLeft>\n"
                            : output = "</DialogRight>\n</DualDialog>\n";
        dialog_state = 0;
      }
      output += "<PageBreak></PageBreak>\n";
      break;
    case ScriptNodeType::ftnBlankLine:
      if (flags & type) {
        break;
      }
      if (dialog_state) {
        dialog_state == 1   ? output = "</Dialog>"
        : dialog_state == 2 ? output = "</DialogLeft>"
                            : output = "</DialogRight></DualDialog>";
        dialog_state = 0;
      }
      output += "<BlankLine></BlankLine>\n";
      break;
    case ScriptNodeType::ftnContinuation:
      if (flags & type) {
        break;
      }
      output = "<Continuation>" + value + "</Continuation>\n";
      break;
    case ScriptNodeType::ftnSceneHeader:
      if (flags & type) {
        break;
      }
      if (!key.empty()) {
        output = "<SceneHeader><SceneNumL>" + key + "</SceneNumL>" + value + "<SceneNumR>" +
                 key + "</SceneNumR></SceneHeader>\n";
      } else {
        output = "<SceneHeader>" + value + "</SceneHeader>\n";
      }
      break;
    case ScriptNodeType::ftnAction:
      if (flags & type) {
        break;
      }
      output = "<Action>" + value + "</Action>\n";
      break;
    case ScriptNodeType::ftnActionCenter:
      if (flags & type) {
        break;
      }
      output = "<ActionCenter>" + value + "</ActionCenter>\n";
      break;
    case ScriptNodeType::ftnTransition:
      if (flags & type) {
        break;
      }
      output = "<Transition>" + value + "</Transition>\n";
      break;
    case ScriptNodeType::ftnDialog:
      if (flags & type) {
        break;
      }
      dialog_state = 1;
      output = "<Dialog>" + key;
      break;
    case ScriptNodeType::ftnDialogLeft:
      if (flags & type) {
        break;
      }
      dialog_state = 2;
      output = "<DualDialog><DialogLeft>" + value;
      break;
    case ScriptNodeType::ftnDialogRight:
      if (flags & type) {
        break;
      }
      dialog_state = 3;
      output = "<DialogRight>" + value;
      break;
    case ScriptNodeType::ftnCharacter:
      if (flags & type) {
        break;
      }
      output = "<Character>" + value + "</Character>\n";
      break;
    case ScriptNodeType::ftnParenthetical:
      if (flags & type) {
        break;
      }
      output = "<Parenthetical>" + value + "</Parenthetical>\n";
      break;
    case ScriptNodeType::ftnSpeech:
      if (flags & type) {
        break;
      }
      output = "<Speech>" + value + "</Speech>\n";
      break;
    case ScriptNodeType::ftnLyric:
      if (flags & type) {
        break;
      }
      output = "<Lyric>" + value + "</Lyric>\n";
      break;
    case ScriptNodeType::ftnNotation:
      if (flags & type) {
        break;
      }
      output = "<Note>" + value + "</Note>\n";
      break;
    case ScriptNodeType::ftnSection:
      if (flags & type) {
        break;
      }
      output = "<SectionH" + key + ">" + value + "</SectionH" + key + ">\n";
      break;
    case ScriptNodeType::ftnSynopsis:
      if (flags & type) {
        break;
      }
      output = "<SynopsisH" + key + ">" + value + "</SynopsisH" + key + ">\n";
      break;
    case ScriptNodeType::ftnUnknown:
    default:
      if (flags & type) {
        break;
      }
      output = "<Unknown>" + value + "</Unknown>\n";
      break;
  }
  return output;
}

std::string Script::to_string(const int &flags) const {
  std::string output{ "<Fountain>\n" };
  for (auto node : nodes) {
    output += node.to_string(flags);
  }
  output += "\n</Fountain>\n";
  return output;
}

std::string Script::parseNodeText(const std::string &input) {
  try {
    static const std::regex re_bolditalic(R"(\*{3}([^*]+?)\*{3})");
    static const std::regex re_bold(R"(\*{2}([^*]+?)\*{2})");
    static const std::regex re_italic(R"(\*{1}([^*]+?)\*{1})");
    static const std::regex re_underline(R"(_([^_\n]+)_)");
    std::string output = std::regex_replace(input, re_bolditalic, "<b><i>$1</i></b>");
    output = std::regex_replace(output, re_bold, "<b>$1</b>");
    output = std::regex_replace(output, re_italic, "<i>$1</i>");
    output = std::regex_replace(output, re_underline, "<u>$1</u>");

    static const std::regex re_note_1(R"(

\[{2}([\S\s]*?)\]

{2})");
    static const std::regex re_note_2(R"(

\[{2}([\S\s]*?)$)");
    static const std::regex re_note_3(R"(^([\S\s]*?)\]

{2})");
    output = std::regex_replace(output, re_note_1, "<note>$1</note>");
    output = std::regex_replace(output, re_note_2, "<note>$1</note>");
    output = std::regex_replace(output, re_note_3, "<note>$1</note>");

    return output;
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
    return input;
  }
}

void Script::clear() {
  nodes.clear();
  curr_node.clear();
}

Script::Script(const std::string &text) {
  parseFountain(text);
}

void ScriptNode::clear() {
  type = ScriptNodeType::ftnUnknown;
  key.clear();
  value.clear();
}

void Script::new_node(
    const ScriptNodeType &type,
    const std::string &key,
    const std::string &value
) {
  end_node();
  curr_node = { type, key, value };
}

void Script::end_node() {
  if (curr_node.type != ScriptNodeType::ftnUnknown) {
    curr_node.value = parseNodeText(curr_node.value);
    nodes.push_back(curr_node);
    curr_node.clear();
  }
}

void Script::append(const std::string &s) {
  if (!curr_node.value.empty()) {
    curr_node.value += '\n';
  }
  curr_node.value += s;
}

}  // namespace Fountain
