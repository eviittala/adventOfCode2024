#include <string>
#include <vector>

namespace common {
std::string readFile(std::string file);
std::string getInputFileDir(const char* compiledFile);
void printVec(const std::vector<int32_t>& vec);
void printVecVec(const std::vector<std::vector<int32_t>>& vec);
}  // namespace common
