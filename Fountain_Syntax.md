# Fountain Syntax

The official description of standard Fountain is available at [Fountain.io](https://fountain.io/syntax).  This library recognizes a modified syntax.  For the most part, the differences do not interfere with the interpretation of standard Fountain.

## Standard Syntax

* Any line not otherwise identified is an Action
  - Forced actions are preceded by a `!`
  - Actions may be centered with `> ... <`
  - Actions may have include multiple consecutive blank lines
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
  - They must start and end on the same line, unless they include a continuation.
* Continuations are lines that contain only whitespace.
  - Blank lines are *not* continuations.
* Sections start with a series of `#`
  - The number of `#` indicate the section number.
* Synopses start with a single `=`

## Modified Syntax

* Continuations include a line with whitespace followed by a period.  This makes continuations visible to the writer.  Some text editors strip trailing whitespace, which would destroy continuations that consist of only whitespace.  If a solo period is needed in the output, it may be escaped or formatted.
* Formatting with `*` and `_` ignore spacing.
  - May be useful to format text mid-word.
  - Use escapes if actual character is needed.
* Multiple blank lines are collapsed into one.
  - They can be forced with continuations if needed.
* Notes may start with `[[` and end with a blank line.  (This is implied by the standard.)
  - Notes may also start with a blank line and end with `]]`.
    - This modification allows two paragraph notes without a continuation.
    - Notes longer than two paragraphs still require continuations.
* The following escapes are recognized:
  - `\*` – Part of standard Fountain.
  - `\[ \]` – In case `[[` and `]]` are wanted in the output.
  - `\_ \: \\ \< \>`
  - `\& \.`
* Scene headings may start with `EST` (establishing).
* Some stock transitions are recognized without forcing the transition.
  - Must be shorter than 20 characters.  (Force if longer is needed.)
  - Ends with " OUT." – FADE OUT. IRIS OUT.
  - Starts with "CUT TO " – CUT TO BLACK.
  - DISSOLVE:
  - END CREDITS:
  - FADE IN:
  - FREEZE FRAME:
  - INTERCUT WITH:
  - IRIS IN:
  - OPENING CREDITS:
  - SPLIT SCREEN:
  - STOCK SHOT:
  - TIME CUT:
  - TITLE OVER:
  - WIPE:
