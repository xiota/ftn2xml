# Fountain Screenplay Processor

This library converts Fountain screenplay documents into XML-style documents with tags that can be easily converted into other markup languages, like HTML or FDX.

## Overview

I needed a fountain parser for a [Geany plugin](https://github.com/xiota/geany-preview).  For performance, the parser had to be written in a compiled language.  However, nearly all existing Fountain parsers are written in scripting languages.  The ones that aren't are either difficult to use with C/C++ or don't work (misidentifying nearly all script elements).  So I was "forced" to write my own.

Calling this a "parser" would be generous since I do not recall how to write a *proper* parser.  Fortunately, the elements of a screenplay are generally identifiable on a line-by-line basis.  So this library works by:

* Identifying the type of each line: Scene Header, Transition, Action, Character, Parenthetical, Speech.
* Using regular expressions to find bold, italics, and underline formatting.

The main complication to this method is dual dialog.  When encountering a dual-dialog marker (`^`), the processor has to go back to modify the most-recently identified dialog node.

## Usage

1. Compile as a library or include the files in your project.
2. Call a function to convert the script into the desired format:

  * `ftn2xml()` – Convert to native XML-style format.
  * `ftn2fdx()` – Convert into Final Draft document.
  * `ftn2screenplain()` – Convert into HTML similar to those produced by screenplain.
  * `ftn2textplay()` – Convert into HTML similar to those produced by textplay.

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
  - They may be forced with `@`
  - There may be a same-line parenthetical.
* Dialog speech follows Character or Parenthetical.
* Parentheticals follow Character or Speech.  They are surrounded with `(` and `)`.
* Bold text is indicated with `**`
  - They must start and end on the same line
* Italic text is indicated with `*`
  - They must start and end on the same line
  - Asterisks may be escaped with `\*`
* Underlined text is indicated with `_`
  - They must start and end on the same line
* Notes are surrounded by `[[` and `]]`
  - They must start and end on the same line, unless they are "continued".
* Continuations are lines that contain only whitespace.
  - Blank lines are *not* continuations.
* Sections start with a series of `#`
  - The number of `#` indicate the section number.
* Synopses start with a single `=`

This library has some differences from the Fountain standard.  For the most part, they do not interfere with interpretation of standard Fountain.

* Continuations include a line with whitespace followed by a period.  This makes continuations visible to the writer.  Some text editors also strip trailing whitespace, which would destroy continuations that consist of only whitespace.  If a solo period is needed in the output, it may be italicized (`*.*`).
* Notes may start with `[[` and end with a blank line.  (This is implied by the standard.)
  - Notes may start with a blank line and end with `]]`.  (So that accidental exclusion of a continuation does not let the end of a note through to the output.) 
* The following escapes are recognized:
  - `\*` – Part of standard Fountain.
  - `\[ \]` – In case `[[` and `]]` are wanted in the output.
  - `\_ \: \\ \< \>`
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

## To do:

* Fix issues with BOM (byte order markers)
* Fix formatting when there are spaces around `*` and `_`
* Export PDF
* Add command-line interface

## License

This software is licensed under the [GPLv3+](License.md).
