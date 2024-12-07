#include "Common.hpp"

#include <fstream>
#include <iostream>
#include <regex>

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

void printVec(const std::vector<int>& vec) {
    for (auto& data : vec) {
        std::cout << data << std::endl;
    }
}

void printVecChar(const std::vector<char>& vec) {
    for (auto& data : vec) {
        std::cout << data << " ";
    }
    std::cout << std::endl;
}

void printVecPair(const std::vector<std::pair<int, int>>& vec) {
    for (auto& data : vec) {
        std::cout << data.first << ", " << data.second << std::endl;
    }
}

void printVecVec(const std::vector<std::vector<int32_t>>& vec) {
    for (const auto& i : vec) {
        for (const auto& j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

std::string removeLineBreaks(const std::string& str) {
    std::regex newlines_re("\n+");
    return std::regex_replace(str, newlines_re, "");
}

uint32_t count(const std::string& str, const char arg) {
    uint32_t count{};
    for (const char c : str) {
        if (c == arg) ++count;
    }
    return count;
}

std::vector<std::vector<uint64_t>> parseUintData(const std::string& str) {
    std::vector<std::vector<uint64_t>> retVec;
    std::istringstream iss{str};
    std::smatch sm;

    for (std::string line; std::getline(iss, line);) {
        std::vector<uint64_t> tVec;
        while (std::regex_search(line, sm, std::regex(R"((\d+))"))) {
            try {
                tVec.push_back(std::stoll(sm[1].str()));
                line = sm.suffix();
            } catch (...) {
                std::cout << "ERROR: " << sm[0].str() << std::endl;
                abort();
            }
        }
        retVec.push_back(std::move(tVec));
    }
    return retVec;
}
}  // namespace common
