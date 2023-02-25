# Fountain Screenplay Processor

This library converts Fountain screenplay documents into XML-style documents with tags that can then be converted into other markup languages, like HTML or FDX.

## Overview

I needed a fountain parser for a [Geany plugin](https://github.com/xiota/geany-preview).  For performance, I wanted a parser written in a compiled language, but nearly all existing Fountain parsers are written in scripting languages.  The ones that aren't are either difficult (for me) to use or were incomplete.  So I was "forced" to write my own.

Calling this a "parser" would be generous since I do not recall how to write a *proper* parser.  Fortunately, the elements of a screenplay are generally identifiable on a line-by-line basis.  So this library works by:

* Identifying the type of each line: Scene Header, Transition, Action, Character, Parenthetical, Speech.
* Using regular expressions to find bold, italics, and underline formatting.

The main complication to this method is dual dialog.  When encountering a dual-dialog marker (`^`), the processor has to go back to modify the most-recently identified dialog node.

## Fountain syntax

A detailed description of standard Fountain is available at [Fountain.io](https://fountain.io/syntax).  This processor recognizes a modified syntax, which is described at [Fountain Syntax](Fountain_Syntax.md).

## Usage

1. Compile as a library or include the files in your project.
   - [podofo](http://podofo.sourceforge.net/) is required for pdf export
2. Call a function to convert the script into the desired format:

   * `ftn2xml()` – Convert to native XML-style format.
   * `ftn2pdf()` – Export to PDF using podofo library.
   * `ftn2fdx()` – Convert into Final Draft document.
   * `ftn2screenplain()` – Convert into HTML similar to those produced by screenplain.
   * `ftn2textplay()` – Convert into HTML similar to those produced by textplay.

## License

This software is licensed under the [GPLv3+](License.md).
