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
More details are contained in the `--help` output.

* `ftn2xml` – Convert to native XML-style format.
* `ftn2html` – Convert to native HTML-style format.
* `ftn2pdf` – Export to PDF using PoDoFo library.
* `ftn2fdx` – Convert into Final Draft document.

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
   - PoDoFo 0.9.x - use the branch [0.0.0-podofo-0.9.x](tree/0.0.0-podofo-0.9.x).
   - PoDoFo 0.10.x - requires [Courier Prime](https://quoteunquoteapps.com/courierprime/) to work around some font issues.

## Building

This project uses `meson` build scripts with a `make` wrapper for convenience.

```bash
git clone https://github.com/xiota/ftn2xml
cd ftn2xml
make all
```

The files will be located in a folder named `pkgdir` for review.  Moving the main `ftn2xml` executable into the `PATH` should be sufficient for use.  System-wide installation is possible with `make sysinstall`.  Removal is possible with `make uninstall`, but not really recommended.

## License

This software is licensed under the GPL-3.0-or-later.
