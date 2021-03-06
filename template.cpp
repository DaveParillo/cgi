//
// A simple cgi program to echo input CGI query string
// Can be used as a CGI template
//
// Compile with: 
// c++ -std=c++11 -Wall -Wextra -pedantic *.cpp -o cgi
//
//
#include "cgi.h"
#include <cstring>
#include <iostream>
#include <string>

std::string version() {
  return "version 0.0.1";
}

static void usage(const char* name) {
  std::cerr << "Usage: " << name << " [-h|--help] [-v] [QUERY_STRING=value]\n";
}

static void help(const char* name) {
  usage(name);
  std::cerr << "Options:\n"
    << "  -h or --help         Show this text and exit\n"
    << "  -v or --version      Show program version and exit\n"
#ifdef HAVE_SETENV
    << "  QUERY_STRING=value   Set the QUERY_STRING in GET request mode.\n\n";
#else
    << "\n";
#endif
  exit(0);
}

// process command line arguments,
// faking a CGI request, if needed.
void process_args(int argc, char** argv);

int main(int argc, char** argv) {
    cgi input;
    size_t count = 0;

    process_args(argc, argv);
    count = input.read();              // fill input.kvp from ENV

    std::cout << jsonify(input.kvp);   // replace this line with your service specific capabilities

    return count;
}

void process_args(int argc, char** argv) {
    for (int i=1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
            help(argv[0]);
        } else if (!std::strcmp(argv[i], "-v") || !std::strcmp(argv[i], "--version")) {
            std::cout << version() << '\n';
            exit(0);
#ifdef HAVE_SETENV
        } else if (std::strncmp(argv[i], "QUERY_STRING", 12) == 0) {
            // for testing, allow setting query string on the command line
            setenv("REQUEST_METHOD", "GET", true);
            std::string qs(argv[i]);
            qs.erase(qs.begin(), qs.begin()+13);
            setenv("QUERY_STRING", qs.c_str(), true);
#endif
        } else {
            std::cerr << "Unknown argument received, " << argv[i] << '\n';
            usage(argv[0]);
            exit(-1);
        }
    }
}

