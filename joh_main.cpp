//
// Created by johma on 25/06/15.
//

#include "joh_main.h"
#include "arg_parser.h"
#include <iostream>
#include <cxxopts.hpp>

int main(int argc, char *argv[]) {
    
    if (parse(argc, argv) != NO_ERROR) {
        std::cerr << "Error parsing arguments." << std::endl;
        return NOTEXPECTED_ERROR;
    }

    return NO_ERROR;
}