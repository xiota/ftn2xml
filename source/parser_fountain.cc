// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#include "parser_fountain.h"

#include <regex>
#include <string>
#include <vector>

#include "model_script.h"
#include "utils_string.h"

namespace Fountain {
namespace {

// --- Helper functions from original fountain.cc ---

bool isForced(const std::string &input) {
  if (input.empty()) {
    return false;
  }
  switch (input[0]) {
    case '#':
    case '=':
    case '>':
    case '!':
    case '.':
    case '(':
    case '@':
    case '*':
    case '_':
    case '~':
      return true;
  }
  return false;
}

bool isTransition(const std::string &input) {
  if (input.empty()) {
    return false;
  }
  const std::size_t len = input.length();
  if (input[0] == '>' && input.back() != '<') {
    return true;
  }
  if (isForced(input)) {
    return false;
  }
  if (len > 20 || len < 5) {
    return false;
  }
  if (!is_upper(input)) {
    return false;
  }

  std::string compare = input.substr(len - 4);
  if (compare == " TO:") {
    return true;
  }
  compare = input.substr(len - 5);
  if (compare == " OUT.") {
    return true;
  }
  compare = input.substr(0, 7);
  if (compare == "CUT TO ") {
    return true;
  }

  const char *transitions[] = {
    "DISSOLVE:",      "END CREDITS:", "FADE IN:",         "FREEZE FRAME:",
    "INTERCUT WITH:", "IRIS IN:",     "OPENING CREDITS:", "SPLIT SCREEN:",
    "STOCK SHOT:",    "TIME CUT:",    "TITLE OVER:",      "WIPE:",
  };
  for (auto str : transitions) {
    if (input == std::string(str)) {
      return true;
    }
  }
  return false;
}

std::string parseTransition(const std::string &input) {
  if (input.empty()) {
    return {};
  }
  if (input[0] == '>') {
    return to_upper(ws_trim(input.substr(1)));
  }
  return to_upper(ws_trim(input));
}

bool isSceneHeader(const std::string &input) {
  if (input.length() < 2) {
    return false;
  }
  if (input[0] == '.' && input[1] != '.') {
    return true;
  }
  if (isForced(input)) {
    return false;
  }
  try {
    static const std::regex re_scene_header(
        R"(^(INT|EXT|EST|INT\.?/EXT|EXT\.?/INT|I/E|E/I)[\.\ ])", std::regex_constants::icase
    );
    if (std::regex_search(input, re_scene_header)) {
      return true;
    }
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }
  return false;
}

auto parseSceneHeader(const std::string &input) {
  std::string first, second;
  bool forced_scene = (input[0] == '.' && input[1] != '.');
  const std::size_t pos = input.find("#");
  if (pos < input.length() - 1 && input.back() == '#') {
    if (forced_scene) {
      first = ws_trim(input.substr(1, pos - 1));
      second = ws_trim(input.substr(pos + 1, input.length() - pos - 3));
    } else {
      first = ws_trim(input.substr(0, pos));
      second = ws_trim(input.substr(pos + 1, input.length() - pos - 2));
    }
  } else {
    first = ws_trim(input.substr(forced_scene ? 1 : 0));
  }
  return std::make_pair(first, second);
}

bool isCenter(const std::string &input) {
  if (input.length() < 2) {
    return false;
  }
  return (input[0] == '>' && input.back() == '<');
}

bool isNotation(const std::string &input) {
  if (input.length() < 4) {
    return false;
  }
  if (input[0] != '[' || input[1] != '[') {
    return false;
  }
  static const std::string strWhiteSpace{ FOUNTAIN_WHITESPACE };
  if (strWhiteSpace.find(input.back()) != std::string::npos) {
    std::string s = ws_rtrim(input);
    if (s[s.length() - 1] == ']' && s[s.length() - 2] == ']') {
      return true;
    }
  } else {
    if (input[input.length() - 1] == ']' && input[input.length() - 2] == ']') {
      return true;
    }
  }
  return false;
}

bool isCharacter(const std::string &input) {
  if (input.empty()) {
    return false;
  }
  if (input[0] == '@') {
    return true;
  }
  if (isForced(input)) {
    return false;
  }
  std::size_t pos = input.find("(");
  if (pos != std::string::npos && input.find(")") != std::string::npos) {
    if (is_upper(input.substr(0, pos))) {
      return true;
    }
  } else if (is_upper(input)) {
    return true;
  }
  return false;
}

std::string parseCharacter(const std::string &input) {
  std::string output;
  if (input[0] == '@') {
    output = ws_ltrim(input.substr(1));
  } else {
    output = input;
  }
  if (output.back() == '^') {
    output = ws_rtrim(output.substr(0, output.length() - 1));
  }
  return output;
}

bool isDualDialog(const std::string &input) {
  return (!input.empty() && input.back() == '^');
}

bool isParenthetical(const std::string &input) {
  if (input.empty()) {
    return false;
  }
  if (input[0] != '(') {
    return false;
  }
  static std::string strWhiteSpace{ FOUNTAIN_WHITESPACE };
  if (strWhiteSpace.find(input.back()) != std::string::npos) {
    std::string s = ws_rtrim(input);
    if (s[s.length() - 1] == ')') {
      return true;
    }
  } else {
    if (input[input.length() - 1] == ')') {
      return true;
    }
  }
  return false;
}

bool isContinuation(const std::string &input) {
  if (input.empty()) {
    return false;
  }
  return (input.find_first_not_of(FOUNTAIN_WHITESPACE) == std::string::npos);
}

auto parseKeyValue(const std::string &input) {
  std::string key, value;
  std::size_t pos = input.find(':');
  if (pos != std::string::npos) {
    key = to_lower(ws_trim(input.substr(0, pos)));
    value = ws_trim(input.substr(pos + 1));
  }
  return std::make_pair(key, value);
}

std::string &parseEscapeSequences_inplace(std::string &input) {
  for (std::size_t pos = 0; pos < input.length();) {
    switch (input[pos]) {
      case '\t':
        input.replace(pos, 1, "    ");
        pos += 4;
        break;
      case '\\':
        if (pos + 1 < input.length()) {
          switch (input[pos + 1]) {
            case '&':
              input.replace(pos, 2, "&#38;");
              pos += 5;
              break;
            case '*':
              input.replace(pos, 2, "&#42;");
              pos += 5;
              break;
            case '_':
              input.replace(pos, 2, "&#95;");
              pos += 5;
              break;
            case ':':
              input.replace(pos, 2, "&#58;");
              pos += 5;
              break;
            case '[':
              input.replace(pos, 2, "&#91;");
              pos += 5;
              break;
            case ']':
              input.replace(pos, 2, "&#93;");
              pos += 5;
              break;
            case '\\':
              input.replace(pos, 2, "&#92;");
              pos += 5;
              break;
            case '<':
              input.replace(pos, 2, "&#60;");
              pos += 5;
              break;
            case '>':
              input.replace(pos, 2, "&#62;");
              pos += 5;
              break;
            case '.':
              input.replace(pos, 2, "&#46;");
              pos += 5;
              break;
            default:
              pos += 2;
              break;
          }
        } else {
          ++pos;
        }
        break;
      default:
        ++pos;
        break;
    }
  }
  return input;
}

}  // namespace

// --- Main parseFountain implementation ---
void Script::parseFountain(const std::string &text) {
  clear();
  if (text.empty()) {
    return;
  }

  std::vector<std::string> lines;
  try {
    static const std::regex re_comment(R"(/\*[\S\s]*?\*/)");
    std::string strTmp = std::regex_replace(text, re_comment, "");
    static const std::regex re_ampersand(R"(([^\\])&(?!#?[a-zA-Z0-9]+;))");
    strTmp = std::regex_replace(strTmp, re_ampersand, "$1&#38;");
    parseEscapeSequences_inplace(strTmp);
    lines = split_lines(strTmp);
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  // determine whether to try to extract header
  bool has_header = false;
  try {
    static const std::regex re_has_header(R"(^[^\s:]+:\s)");
    if (std::regex_search(text, re_has_header)) {
      has_header = true;
    }
  } catch (std::regex_error &e) {
    print_regex_error(e, __FILE__, __LINE__);
  }

  int currSection = 1;  // used for synopsis

  for (auto &line : lines) {
    std::string s = ws_ltrim(line);

    if (has_header) {
      if (s.find(':') != std::string::npos) {
        metadata[curr_node.key] = trim_inplace(curr_node.value);
        auto kv = parseKeyValue(s);
        if (!kv.first.empty()) {
          new_node(ScriptNodeType::ftnKeyValue, kv.first);
        }
        if (!kv.second.empty()) {
          append(kv.second);
        }
        continue;
      }
      if (line.empty()) {
        metadata[curr_node.key] = trim_inplace(curr_node.value);
        end_node();
        has_header = false;
        continue;
      } else {
        trim_inplace(s);
        if (!s.empty()) {
          append(s);
        }
        continue;
      }
    }

    // Blank Line
    if (line.empty()) {
      new_node(ScriptNodeType::ftnBlankLine, line);
      end_node();
      continue;
    }

    // Continuation
    if (isContinuation(line)) {
      if (curr_node.type != ScriptNodeType::ftnUnknown) {
        append(" ");
      } else {
        new_node(ScriptNodeType::ftnContinuation, line);
        end_node();
      }
      continue;
    }

    // Notation
    if (isNotation(s)) {
      if (line[0] == ' ' || line[0] == '\t' || line.back() == ' ' || line.back() == '\t') {
        append(s);
      } else {
        append(s);
        new_node(ScriptNodeType::ftnBlankLine);
        end_node();
      }
      continue;
    }

    // Page Break
    if (begins_with(s, "===")) {
      new_node(ScriptNodeType::ftnPageBreak, s);
      end_node();
      continue;
    }

    // Transition
    if (curr_node.type == ScriptNodeType::ftnUnknown && isTransition(s)) {
      new_node(ScriptNodeType::ftnTransition);
      append(parseTransition(s));
      end_node();
      continue;
    }

    // Scene Header
    if (curr_node.type == ScriptNodeType::ftnUnknown && isSceneHeader(s)) {
      auto scene = parseSceneHeader(s);
      new_node(ScriptNodeType::ftnSceneHeader, scene.second);
      append(scene.first);
      end_node();
      continue;
    }

    // Parenthetical
    if (isParenthetical(s)) {
      if (!nodes.empty()) {
        ScriptNodeType ct = nodes.back().type;
        if (ct == ScriptNodeType::ftnParenthetical || ct == ScriptNodeType::ftnCharacter ||
            ct == ScriptNodeType::ftnSpeech) {
          new_node(ScriptNodeType::ftnParenthetical);
          append(ws_trim(s));
          end_node();
          continue;
        }
      }
    }

    // Speech / Lyric
    if (curr_node.type == ScriptNodeType::ftnSpeech) {
      if (s.length() > 1 && s[0] == '~') {
        new_node(ScriptNodeType::ftnLyric);
        append(s.substr(1));
        continue;
      } else {
        append(s);
        continue;
      }
    } else if (curr_node.type == ScriptNodeType::ftnLyric) {
      if (s.length() > 1 && s[0] == '~') {
        append(s.substr(1));
        continue;
      } else {
        new_node(ScriptNodeType::ftnSpeech);
        append(s);
        continue;
      }
    } else if (!nodes.empty()) {
      ScriptNodeType ct = nodes.back().type;
      if (ct == ScriptNodeType::ftnParenthetical || ct == ScriptNodeType::ftnCharacter) {
        if (s.length() > 1 && s[0] == '~') {
          new_node(ScriptNodeType::ftnLyric);
          append(ws_ltrim(s.substr(1)));
          continue;
        } else {
          new_node(ScriptNodeType::ftnSpeech);
          append(s);
          continue;
        }
      }
    }

    // Character
    if (curr_node.type == ScriptNodeType::ftnUnknown && isCharacter(s)) {
      if (isDualDialog(s)) {
        bool prev_dialog_modded = false;
        for (std::size_t pos = nodes.size(); pos-- > 0;) {
          if (nodes[pos].type == ScriptNodeType::ftnDialog) {
            nodes[pos].type = ScriptNodeType::ftnDialogLeft;
            prev_dialog_modded = true;
            break;
          }
        }
        if (prev_dialog_modded) {
          new_node(ScriptNodeType::ftnDialogRight);
        } else {
          new_node(ScriptNodeType::ftnDialog);
        }
        new_node(ScriptNodeType::ftnCharacter);
        append(parseCharacter(s));
        end_node();
      } else {
        new_node(ScriptNodeType::ftnDialog);
        new_node(ScriptNodeType::ftnCharacter);
        append(parseCharacter(s));
        end_node();
      }
      continue;
    }

    // Isolated Lyric
    if (s.length() > 1 && s[0] == '~') {
      new_node(ScriptNodeType::ftnLyric);
      append(ws_ltrim(s.substr(1)));
      end_node();
      continue;
    }

    // Section
    if (!s.empty() && s[0] == '#') {
      for (std::size_t i = 1; i < 6; ++i) {
        if (s.length() > i && s[i] == '#') {
          if (i == 5) {
            new_node(ScriptNodeType::ftnSection, std::to_string(i + 1));
            append(s.substr(i + 1));
            currSection = i + 1;
            break;
          }
        } else {
          new_node(ScriptNodeType::ftnSection, std::to_string(i));
          append(s.substr(i));
          currSection = i;
          break;
        }
      }
      end_node();
      continue;
    }

    // Synopsis
    if (s.length() > 1 && s[0] == '=') {
      new_node(ScriptNodeType::ftnSynopsis, std::to_string(currSection));
      append(ws_trim(s.substr(1)));
      end_node();
      continue;
    }

    // Action
    if (curr_node.type == ScriptNodeType::ftnAction) {
      if (isCenter(s)) {
        new_node(ScriptNodeType::ftnActionCenter);
        append(ws_trim(s.substr(1, s.length() - 2)));
        end_node();
      } else {
        append(line);
      }
      continue;
    }
    if (isCenter(s)) {
      new_node(ScriptNodeType::ftnActionCenter);
      append(ws_trim(s.substr(1, s.length() - 2)));
      end_node();
      continue;
    }
    if (s.length() > 1 && s[0] == '!') {
      new_node(ScriptNodeType::ftnAction);
      append(s.substr(1));
      continue;
    }
    if (curr_node.type == ScriptNodeType::ftnUnknown) {
      new_node(ScriptNodeType::ftnAction);
      append(line);
      continue;
    }
  }

  end_node();
}

}  // namespace Fountain
