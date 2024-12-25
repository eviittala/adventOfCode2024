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

void doCmd(const uint64_t opcode, const uint64_t combo, Registers& regs,
           size_t& pc, auto& vec) {
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
            regs.B ^= combo;
        } break;
        case 2: {
            regs.B = getOperand() % 8;
        } break;
        case 3: {
            if (regs.A != 0) {
                pc = combo;
                return;
            }
        } break;
        case 4: {
            regs.B = regs.B ^ regs.C;
        } break;
        case 5: {
            std::string str = std::to_string(getOperand() % 8);
            for (size_t i{}; i < str.size(); ++i) {
                vec.push_back(str[i] - '0');
            }
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
}

std::string makeOutput(const std::vector<uint64_t>& vec) {
    std::string ret;
    for (size_t i{}; i < vec.size(); ++i) {
        ret += std::to_string(vec.at(i)) + ", ";
    }
    return ret;
}

uint64_t getRegA(std::vector<uint64_t> prog, const uint64_t val) {
    if (prog.empty()) {
        return val;
    }

    for (uint64_t i{}; i < 8; ++i) {
        const uint64_t regA = val << 3 | i;
        uint64_t regB = regA % 8;      // 0
        regB = regB ^ 2;               // 1
        uint64_t regC = regA >> regB;  // 2
        // regA = regA / 8;                // 3
        regB = regB ^ 0x7;              // 4
        regB = regB ^ regC;             // 5
        if (regB % 8 == prog.back()) {  // 6
            std::vector<uint64_t> prog_t(std::begin(prog), std::end(prog) - 1);
            const uint64_t val_t = getRegA(prog_t, regA);
            if (val_t == UINT64_MAX) continue;
            return val_t;
        }
    }
    return UINT64_MAX;
}

uint64_t getResult(const std::string& str) {
    // auto regs = parseRegisters(str);
    Registers regs;
    auto prog = parseProgram(str);
    const uint64_t val = getRegA(prog, 0);
    std::vector<uint64_t> output;

    regs.A = val;
    regs.B = 0;
    regs.C = 0;
    size_t pc{};
    while (pc < prog.size()) {
        const uint64_t opcode = prog.at(pc);
        const uint64_t combo = prog.at(pc + 1);
        doCmd(opcode, combo, regs, pc, output);
    }

    if (output != prog) {
        std::cout << "Incorrect result! Output: " << makeOutput(output).c_str()
                  << std::endl;
    }
    return val;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
