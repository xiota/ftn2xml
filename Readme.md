# Fountain Screenplay Processor

This library converts Fountain screenplay documents into XML-style documents with unique tags that can be converted into other markup languages, like HTML or FDX.

## Overview

I needed a fountain parser for a [Geany plugin](https://github.com/xiota/geany-preview).  For performance, the parser had to be written in a compiled language.  However, nearly all existing Fountain parsers are written in scripting languages.  The ones that aren't either don't work or are difficult to use with C/C++.  So I was "forced" to write my own.

Calling this a "parser" would be generous since I do not recall how to write a "proper" parser.  Fortunately, the elements of a screenplay are generally identifiable on a line-by-line basis.  So this library works by:

* Identifying the type of each line: Scene Header, Transition, Action, Character, Parenthetical, Speech.
* Using regular expressions to find bold, italics, and underlining formatting.  This will need to be replaced with a character-by-character parser to handle handle escape sequences.

The main complication to this method is dual dialog.  When encountering a dual-dialog marker (`^`), the processor has to goes back to modify the most-recently identified dialog node.

## Usage

1. Compile as a library or include the files into your project.
2. Call a function to convert the script into the desired format:

  * `ftn2xml()` – Convert to native XML-style format.
  * `ftn2fdx()` – Convert into Final Draft document.
  * `ftn2screenplain()` – Convert into HTML similar to those produced by screenplain.
  * `ftn2plain()` – Convert into HTML similar to those produced by textplay.

## Fountain syntax

A detailed description of Fountain syntax is available at [Fountain.io](https://fountain.io/syntax).

* Any line not otherwise identified is an Action
	- Forced actions are preceded by a `!`
	- Actions may be centered with `> ... <`
* Comments are surrounded by `/*` and `*/`
  - Blank lines are included
* Tabs are replaced with four spaces
* Indentation is ignored except in Action
* Pagebreaks are indicated with `===`
* Scene headings start with `INT`, `EXT`, or `I/E`.
  - They may be forced with `.`
* Transitions end with ` TO:`
  - They may be forced with `>`
* Lyrics are preceded by `~`
* Characters are in all upper case.
  - They may be followed by a parenthetical.
  - They may be forced with `@`
* Dialogs follow Character or Parenthetical.
* Parentheticals follow Character or Speech.
* Speech follow Character or Parenthetical. 
* Bold text is indicated with `**`
  - They must start and end on the same line
* Italic text is indicated with `*`
  - They must start and end on the same line
* Underlined text is indicated with `_`
  - They must start and end on the same line
* Notes are surrounded by `[[` and `]]`
  - They must start and end on the same line, unless they are "continued".
* Continuations are lines that contain only whitespace.
* Sections start with a series of `#`
  - Corresponding synopses start with a single `=`

This library implements some changes to the Fountain standard:

* Continuations include a line with whitespace followed by a period.  This is to make continuations visible to the writer.  Some editors also strip trailing whitespace, which would destroy continuations that consist of only whitespace.  If a solo period is needed in the output, it may be italicized (`*.*`).
*  Notes may start with `[[` and end with a blank line.  (This is implied by the standard.)
  - Notes may start with a blank line and end with `]]`.
* Scene headings may start with `EST` (establishing).
* Some stock transitions are recognized without forcing the transition.
  - CUT TO BLACK:
  - DISSOLVE:
  - END CREDITS:
  - FADE IN:
  - FADE OUT.
  - FREEZE FRAME:
  - INTERCUT WITH:
  - IRIS IN:
  - IRIS OUT.
  - OPENING CREDITS:
  - SPLIT SCREEN:
  - STOCK SHOT:
  - TIME CUT:
  - TITLE OVER:
  - WIPE:

## Known Issues

* Doesn't handle escaped `\*` and `\_`.
* Doesn't work with BOM (byte order markers)

## To do:

* Fix known issues
* PDF export.
* Create command line interface to convert files.

## License

This software is licensed under the [GPLv3+](License.md).
