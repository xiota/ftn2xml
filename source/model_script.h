// SPDX-FileCopyrightText: Copyright 2021-2025 xiota
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <map>
#include <string>
#include <vector>

#include "config.h"

namespace Fountain {

enum ScriptNodeType : unsigned long long {
  ftnNone = 0,
  ftnUnknown = 1ull,
  ftnBoneyard = 1ull << 1,
  ftnComment = 1ull << 2,
  ftnKeyValue = 1ull << 3,
  ftnContinuation = 1ull << 4,
  ftnPageBreak = 1ull << 5,
  ftnBlankLine = 1ull << 6,
  ftnSceneHeader = 1ull << 7,
  ftnAction = 1ull << 8,
  ftnActionCenter = 1ull << 9,
  ftnTransition = 1ull << 10,
  ftnDialog = 1ull << 11,
  ftnDialogLeft = 1ull << 12,
  ftnDialogRight = 1ull << 13,
  ftnCharacter = 1ull << 14,
  ftnParenthetical = 1ull << 15,
  ftnSpeech = 1ull << 16,
  ftnNotation = 1ull << 17,
  ftnLyric = 1ull << 18,
  ftnSection = 1ull << 19,
  ftnSynopsis = 1ull << 20,
};

class ScriptNode {
 public:
  std::string to_string(const int &flags = ScriptNodeType::ftnNone) const;
  void clear();

  ScriptNodeType type = ScriptNodeType::ftnUnknown;
  std::string key;
  std::string value;
};

class Script {
 public:
  Script() = default;
  explicit Script(const std::string &text);

  void clear();
  void parseFountain(const std::string &text);
  std::string to_string(const int &flags = ScriptNodeType::ftnNone) const;

  std::vector<ScriptNode> nodes;
  std::map<std::string, std::string> metadata;

 private:
  ScriptNode curr_node;
  std::string parseNodeText(const std::string &input);
  void new_node(
      const ScriptNodeType &type,
      const std::string &key = "",
      const std::string &value = ""
  );
  void end_node();
  void append(const std::string &s);
};

}  // namespace Fountain
