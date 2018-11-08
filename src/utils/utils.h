//
// Created by xuyz on 2018/11/8.
//

#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <iostream>
#include <fstream>
std::string read_file(const char* path){
    std::ifstream t(path);
    return std::string((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
}
#endif //UTILS_UTILS_H
