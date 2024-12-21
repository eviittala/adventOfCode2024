#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <unordered_map>

#include "Common.hpp"

struct Registers {
    uint64_t A{};
    uint64_t B{};
    uint64_t C{};
};

Registers parseRegisters(const std::string& str) {
    std::regex reA(R"(Register A:\s(\d+))");
    std::regex reB(R"(Register B:\s(\d+))");
    std::regex reC(R"(Register C:\s(\d+))");
    std::smatch sm;
    Registers ret;

    if (std::regex_search(str, sm, reA)) {
        ret.A = std::stoll(sm[1].str());
    }

    if (std::regex_search(str, sm, reB)) {
        ret.B = std::stoll(sm[1].str());
    }

    if (std::regex_search(str, sm, reC)) {
        ret.C = std::stoll(sm[1].str());
    }

    return ret;
}

std::vector<uint64_t> parseProgram(const std::string& str) {
    std::regex re(R"(Program:\s(.*))");
    std::smatch sm;
    std::vector<uint64_t> ret;

    if (std::regex_search(str, sm, re)) {
        // std::cout << sm[1].str() << std::endl;
        std::string temp = sm[1].str();
        while (std::regex_search(temp, sm, std::regex(R"((\d+))"))) {
            ret.push_back(std::stoll(sm[1].str()));
            temp = sm.suffix();
        }
    }

    return ret;
}

std::vector<uint64_t> doCmd(const uint64_t opcode, const uint64_t combo,
                            Registers& regs, size_t& pc) {
    auto getOperand = [&regs, &combo]() -> uint64_t {
        if (combo < 4) return combo;
        switch (combo) {
            case 4:
                return regs.A;
            case 5:
                return regs.B;
            case 6:
                return regs.B;
        }
        return combo;
    };
    switch (opcode) {
        case 0: {
            const uint64_t num = regs.A;
            regs.A = num / std::pow(2, getOperand());
        } break;
        case 1: {
            regs.B ^= getOperand();
        } break;
        case 2: {
            regs.B = getOperand() % 8;
        } break;
        case 3: {
            if (regs.A != 0) {
                pc = combo;
                return {};
            }
        } break;
        case 4: {
            regs.B = regs.B ^ regs.C;
        } break;
        case 5: {
            std::string str = std::to_string(getOperand() % 8);
            std::vector<uint64_t> ret;
            for (size_t i{}; i < str.size(); ++i) {
                ret.push_back(str[i] - '0');
            }
            pc += 2;
            return ret;
        } break;
        case 6: {
            const uint64_t num = regs.A;
            regs.B = num / std::pow(2, getOperand());
        } break;
        case 7: {
            const uint64_t num = regs.A;
            regs.C = num / std::pow(2, getOperand());
        } break;
    }
    pc += 2;
    return {};
}

void getResult(const std::string& str) {
    auto regs = parseRegisters(str);
    auto prog = parseProgram(str);
    size_t pc{};

    // printf("Register A:%lu\nRegister B:%lu\nRegister C:%lu\n", regs.A,
    // regs.B,
    //        regs.C);
    // std::cout << "Program: ";
    // for (auto i : prog) {
    //     std::cout << i << ", ";
    // }
    // std::cout << std::endl;
    std::vector<uint64_t> output;

    while (pc < prog.size()) {
        const uint64_t opcode = prog.at(pc);
        const uint64_t combo = prog.at(pc + 1);
        const auto res = doCmd(opcode, combo, regs, pc);
        std::copy(std::begin(res), std::end(res), std::back_inserter(output));
    }

    for (size_t i{}; i < output.size(); ++i) {
        std::cout << output.at(i);
        if ((i + 1) < output.size()) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: ";
    getResult(str);
    return 0;
}
