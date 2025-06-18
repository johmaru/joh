//
// Created by johma on 25/06/15.
//

#ifndef JOH_MAIN_H
#define JOH_MAIN_H

#include "arg_parser.h"

enum OSType {
    OS_UNKNOWN = 0,
    OS_WINDOWS = 1,
    OS_LINUX = 2
};

#if defined (_WIN32) || defined (_WIN64)
    #define JOH_PLATFORM_WINDOWS
#elif defined (__linux__)
    #define JOH_PLATFORM_LINUX
#else
    #define JOH_PLATFORM_UNKNOWN
#endif

#ifdef JOH_PLATFORM_WINDOWS
    #define JOH_PLATFORM_NAME OS_WINDOWS
    #define JOH_PATH_SEPARATOR '\\'
    #define JOH_LINE_ENDING "\r\n"
    #define JOH_DYNAMIC_LIB_EXT ".dll"
#elif defined JOH_PLATFORM_LINUX
    #define JOH_PLATFORM_NAME OS_LINUX
    #define JOH_PATH_SEPARATOR '/'
    #define JOH_LINE_ENDING "\n"
    #define JOH_DYNAMIC_LIB_EXT ".so"
#else
    #define JOH_PLATFORM_NAME OS_UNKNOWN
#endif



enum MainErrorType {
    NO_ERROR = 0,
    NOTEXPECTED_ERROR = 1
};

class joh_main {
    public:

        static constexpr const char* VERSION = "0.1.0";

        static OSType getPlatformName() {
            return JOH_PLATFORM_NAME;
        }

        static char getPathSeparator() {
            return JOH_PATH_SEPARATOR;
        }

        static const char* getLineEnding() {
            return JOH_LINE_ENDING;
        }

};



#endif //JOH_MAIN_H
