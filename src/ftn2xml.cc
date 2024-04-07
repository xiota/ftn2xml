#include "ftn2xml.h"

#include <stdio.h>

#include <CLI/CLI.hpp>
#include <fstream>
#include <string>

#include "auxiliary.h"
#include "fountain.h"

int main(int argc, char** argv) {
  auto cmd = std::string{argv[0]};
  cmd = cmd.substr(cmd.find_last_of("/\\") + 1);

  CLI::App app;

  std::string input;
  app.add_option("-i, --input", input, "input file, default stdin")
      ->default_val("/dev/stdin")
      ->option_text("<file>");

  std::string output_file;
  app.add_option("-o, --output", output_file, "output file, default stdout")
      ->default_val("/dev/stdout")
      ->option_text("<file>");

  app.set_version_flag("-V, --version", cmd + " " VERSION,
                       "Print version information and exit");

  app.set_help_flag("-h, --help", "Print this help message and exit");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& e) {
    return app.exit(e);
  }

  // read input file and check not empty
  input = file_get_contents(input);
  if (input.empty()) {
    std::cerr << "Error reading file or input empty." << std::endl;
    return 1;
  }

  // execute desired action
  std::string output;
  if (cmd.find("ftn2pdf") != std::string::npos) {
    Fountain::ftn2pdf(output_file, input);
    return 0;
  } else if (cmd.find("ftn2html") != std::string::npos) {
    output = Fountain::ftn2html(input);
  } else if (cmd.find("ftn2fdx") != std::string::npos) {
    output = Fountain::ftn2fdx(input);
  } else {
    // default is ftn2xml
    output = Fountain::ftn2xml(input);
  }

  file_set_contents(output_file, output);

  return 0;
}
