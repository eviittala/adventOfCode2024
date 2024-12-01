#include "Common.hpp"

#include <fstream>
#include <iostream>

namespace common {
std::string readFile(std::string file) {
    if (std::ifstream is{file, std::ios::ate}) {
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        is.read(&str[0], size);
        return str;
    }
    printf("Cannot open file: %s\n", file.c_str());
    return {};
}

std::string getInputFileDir(const char* compiledFile) {
    std::string fileDir = compiledFile;
    const std::string dir = fileDir.substr(0, fileDir.find_last_of('/'));
    return dir + "/" + "Input.txt";
}

void printVec(const std::vector<int32_t>& vec) {
    for (auto& data : vec) {
        std::cout << data << std::endl;
    }
}
}  // namespace common
