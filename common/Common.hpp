#include <cstdint>
#include <string>
#include <vector>

namespace common {
std::string readFile(std::string file);
std::string getInputFileDir(const char* compiledFile);
void printVec(const std::vector<int>& vec);
void printVecPair(const std::vector<std::pair<int, int>>& vec);
void printVecVec(const std::vector<std::vector<int32_t>>& vec);
std::string removeLineBreaks(const std::string& str);
uint32_t count(const std::string& str, const char arg);
}  // namespace common
