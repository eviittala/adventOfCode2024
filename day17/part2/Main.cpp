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

uint64_t& getOperand(Registers& regs, uint64_t& combo) {
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
}
// 2,4 : 1,2 : 7,5 : 0,3 : 1,7 : 4,1 : 5,5 : 3,0
// 00  : 02  : 04  : 06  : 08  : 0A  : 0C  : 0E
// 00: B <= A % 8
// 02: B <= B ^ 2
// 04: C <= A / 2^B
// 06: A <= A / 2^3
// 08: B <= B ^ 7
// 0A: B <= B ^ C
// 0C: out <= B % 8
// 0E: jnz <= if A zero

std::vector<uint64_t> doCmd(const uint64_t opcode, uint64_t combo,
                            Registers& regs, const uint64_t val) {
    switch (opcode) {
        case 0: {
            const uint64_t num = regs.A;
            regs.A = num * std::pow(2, getOperand(regs, combo));
        } break;
        case 1: {
            regs.B |= combo;
        } break;
        case 2: {
            regs.B = getOperand(regs, combo) << 3;
        } break;
        case 3: {
            // if (regs.A != 0) {
            //     // pc = combo;
            //     return {};
            // }
        } break;
        case 4: {
            regs.C = regs.B | regs.C;
        } break;
        case 5: {
            // TODO eero
            uint64_t& operand = getOperand(regs, combo);
            operand += val;
        } break;
        case 6: {
            const uint64_t num = regs.A;
            regs.B = num * std::pow(2, getOperand(regs, combo));
        } break;
        case 7: {
            const uint64_t num = regs.A;
            regs.C = num * std::pow(2, getOperand(regs, combo));
        } break;
    }
    return {};
}

void doCmdInverse(Registers& regs, std::vector<uint64_t>& prog,
                  const int indx) {
    if (indx < 0) return;
    const uint64_t val = prog.at(indx);
    for (int i = prog.size() - 2; 0 <= i; i -= 2) {
        const uint64_t opcode = prog[i];
        const uint64_t combo = prog[i + 1];
        doCmd(opcode, combo, regs, val);
        //        if (opcode == 0) {
        //            regs.A *= (std::pow(2, getOperand(regs, combo)));
        //        }
        //        if (opcode == 5) {
        //            regs.A += val;
        //        }
        //        if (opcode == 1)
        //        {
        //
        //        }

        std::cout << "RegA: " << regs.A << ", " << "RegB: " << regs.B << ", "
                  << "RegC: " << regs.C << ", " << indx << ", val: " << val
                  << ", " << "opcode: " << opcode << " combo: " << combo
                  << std::endl;

        /*
        uint64_t ret{};
        for (auto it = std::rbegin(prog); it != std::rend(prog); ++it) {
            if ((it + 1) != std::rend(prog))
                ret = ret * 8 + *(it + 1);
            else
                ret *= 8;
            std::cout << "Ret: " << ret << std::endl;
        }
        return ret;
        */
    }
    doCmdInverse(regs, prog, indx - 1);
}

uint64_t getResult(const std::string& str) {
    // auto regs = parseRegisters(str);
    Registers regs;
    auto prog = parseProgram(str);

    // printf("Register A:%lu\nRegister B:%lu\nRegister C:%lu\n", regs.A,
    // regs.B,
    //        regs.C);
    // std::cout << "Program: ";
    // for (auto i : prog) {
    //     std::cout << i << ", ";
    // }
    // std::cout << std::endl;
    std::vector<uint64_t> output;
    doCmdInverse(regs, prog, prog.size() - 1);
    return regs.A;

    //    while (pc < prog.size()) {
    //        const uint64_t opcode = prog.at(pc);
    //        const uint64_t combo = prog.at(pc + 1);
    //        const auto res = doCmd(opcode, combo, regs, pc);
    //        std::copy(std::begin(res), std::end(res),
    //        std::back_inserter(output));
    //    }
    //
    //    for (size_t i{}; i < output.size(); ++i) {
    //        std::cout << output.at(i);
    //        if ((i + 1) < output.size()) {
    //            std::cout << ",";
    //        }
    //    }
    //    std::cout << std::endl;
}

int main(int args, char* argv[]) {
    const auto str = common::readFile(common::getInputFileDir(__FILE__));
    std::cout << "Result: " << getResult(str) << std::endl;
    return 0;
}
