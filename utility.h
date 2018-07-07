//
// Copyright @ Somdip Dey, 2018
//

#ifndef OBJECTIFY_UTILITY_H
#define OBJECTIFY_UTILITY_H

#include "opencv2/opencv.hpp"

#include <thread>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

namespace utility {

    inline const char * currentDirectory() {
        char currentPath[FILENAME_MAX];
        if (!GetCurrentDir(currentPath, sizeof(currentPath))) {
            return nullptr;
        }
        currentPath[sizeof(currentPath) - 1] = '\0';
        std::string path(currentPath);
        return path.c_str();
    }

    inline void replaceStringInPlace(std::string& subject,
                                     const std::string& search,
                                     const std::string& replace) {
        size_t pos = 0;
        while((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }

    inline void sleep(int seconds) {
        std::chrono::milliseconds timespan(seconds); // or whatever
        std::this_thread::sleep_for(timespan);
    }
    
    bool is_number(const std::string& s)
    {
        return !s.empty() && std::find_if(s.begin(),
                                          s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

}


#endif //OBJECTIFY_UTILITY_H
