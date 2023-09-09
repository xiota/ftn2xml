#include <stdio.h>

#include <fstream>
#include <string>

#include "auxiliary.h"
#include "fountain.h"

void print_help(std::string cmd) {
  if (cmd.find("ftn2pdf") != std::string::npos) {
    cmd = "ftn2pdf";
    printf("Usage: %s <infile> <outfile>\n", cmd.c_str());
    return;
  } else if (cmd.find("ftn2html") != std::string::npos) {
    cmd = "ftn2html";
  } else if (cmd.find("ftn2fdx") != std::string::npos) {
    cmd = "ftn2fdx";
  } else {
    cmd = "ftn2xml";
  }

  printf("Usage: %s <infile> [outfile]\n", cmd.c_str());
  return;
}

int main(int argc, char** argv) {
  // check correct arguments
  if (argc < 2 || argc > 3) {
    print_help(argv[0]);
    return 0;
  }

  // read input file and check not empty
  std::string input = file_get_contents(argv[1]);

  if (input.empty()) {
    print_help(argv[0]);
    return 0;
  }

  // execute desired action
  std::string cmd{argv[0]};
  std::string output;

  if (cmd.find("ftn2pdf") != std::string::npos) {
    // need output file for pdf
    if (argc != 3) {
      print_help(argv[0]);
      return 0;
    }

    Fountain::ftn2pdf(argv[2], input);
    return 0;
  } else if (cmd.find("ftn2html") != std::string::npos) {
    output = Fountain::ftn2html(input);
  } else if (cmd.find("ftn2fdx") != std::string::npos) {
    output = Fountain::ftn2fdx(input);
  } else {  // default is ftn2xml
    output = Fountain::ftn2xml(input);
  }

  // output to file or stdout
  if (argc == 3) {
    file_set_contents(argv[2], output);
  } else {
    printf("%s", output.c_str());
  }

  return 0;
}
