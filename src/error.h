#ifndef ERROR_H
#define ERROR_H

#include <cstdlib>
#include <iostream>
#include <string>

inline
void error (std::string msg) {
    std::cerr << msg << std::endl;
    std::exit(1);
}

#endif//ERROR_H
