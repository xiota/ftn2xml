# Fountain Screenplay Processor (`ftn2xml`)

This library converts Fountain screenplay documents into XML-style documents with tags that can be converted to other markup languages, like HTML or FDX.  Some command-line utilities are provided to demonstrate use, including PDF conversion using PoDoFo.

## Overview

I needed a fountain parser for a [Geany plugin](https://github.com/xiota/geany-preview).  For performance, I wanted a parser written in a compiled language, but nearly all existing (open-source) Fountain parsers are written in scripting languages.  The ones that aren't are either difficult (for me) to use or were incomplete.  So I was "forced" to write my own.

Calling this a "parser" would be generous since I do not recall how to write a *proper* parser.  Fortunately, the elements of a screenplay are generally identifiable on a line-by-line basis.  So this library works by:

* Identifying the type of each line: Scene Header, Transition, Action, Character, Parenthetical, Speech.
* Using regular expressions to find bold, italics, and underline formatting.

The main complication to this method is dual dialog.  When encountering a dual-dialog marker (`^`), the processor has to go back to modify the most-recently identified dialog node.

## Fountain syntax

A detailed description of standard Fountain is available at [Fountain.io](https://fountain.io/syntax).  This processor recognizes a modified syntax, which is described at [Fountain Syntax](Fountain_Syntax.md).

## Usage (command line)

Some basic command line utilities are provided.

* `ftn2xml` – Convert to native XML-style format.
* `ftn2html` – Convert to native HTML-style format.
* `ftn2pdf` – Export to PDF using PoDoFo library.
* `ftn2fdx` – Convert into Final Draft document.

Usage is described in the output of the `--help` option.

## Usage (source code)

1. Compile as a library or include the files in your project.
2. `#include "fountain.h"`
3. Call a function to convert the script into the desired format:

   * `ftn2xml()` – Convert to native XML-style format.
   * `ftn2html()` – Convert to native HTML-style format.
   * `ftn2pdf()` – Export to PDF using podofo library.
   * `ftn2fdx()` – Convert into Final Draft document.
   * `ftn2screenplain()` – Convert into HTML similar to those produced by screenplain.
   * `ftn2textplay()` – Convert into HTML similar to those produced by textplay.

## Requirements

* Compiler that supports C++17 standard.  Both `clang++` and `g++` seem to work.
* [CLI11](https://github.com/CLIUtils/CLI11) for option parsing.
* [PoDoFo](https://github.com/podofo/podofo) for PDF export.
   - PoDoFo 0.9.x - use commit tagged [0.0.2](releases/tag/v0.0.2).
   - PoDoFo 0.10.x - requires [Courier Prime](https://quoteunquoteapps.com/courierprime/) to work around some font issues.

## Building

```bash
git clone https://github.com/xiota/ftn2xml
cmake -B build -S ftn2xml -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
DESTDIR=output cmake --install
```


## License

This software is licensed under the GPL-3.0-or-later.
