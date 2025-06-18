#include <iostream>
#include <cxxopts.hpp>
#include "arg_parser.h"
#include "fs_system.h"
#include "joh_main.h"

int parse(int argc, char *argv[]) {
    cxxopts::Options options("joh", "Convinience tool for anything");

    options.add_options()
        ("h,help", "Print help")
        ("v,version", "Print version")
        ("c,config", "Path to config file", cxxopts::value<std::string>()->default_value("joh_config.json"));

    try {
        auto result = options.parse(argc, argv);

        if (settings_init() != FS_NO_ERROR) {
            std::cerr << "Error initializing settings." << std::endl;
            return AP_ERROR_INVALID_ARGUMENT;
        }

        if (argc == 1 || (argc == 2 && std::string(argv[1]) == "help")) {
            std::cout << options.help() << std::endl;
            return AP_NO_ERROR;
        }

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return AP_NO_ERROR;
        }

        if (result.count("version")) {
            std::cout << "joh version : " << joh_main::VERSION << std::endl;
            return AP_NO_ERROR;
        }

        if (result.count("config")) {
            std::string configPath = result["config"].as<std::string>();
            std::cout << "Using config file: " << configPath << std::endl;
            // Load and use the config file here
        }

        if (result.unmatched().size() > 0) {
            std::cerr << "Unrecognized options: ";
            for (const auto &option : result.unmatched()) {
                std::cerr << option << " ";
            }
            std::cerr << std::endl;
            return AP_ERROR_INVALID_ARGUMENT;
        }

    } catch (const cxxopts::exceptions::exception &e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return AP_ERROR_CXXOPTS_ERROR;
    }

    return 0;
}