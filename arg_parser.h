#ifndef ARG_PARSER_H
#define ARG_PARSER_H

enum ArgumentParserErrorType {
    AP_NO_ERROR = 100,
    AP_ERROR_INVALID_ARGUMENT = 101,
    AP_ERROR_CXXOPTS_ERROR = 102,
};

int parse(int argc, char *argv[]);

#endif