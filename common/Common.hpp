#include <cstdint>
#include <string>
#include <vector>

namespace common {
std::string readFile(std::string file);
std::string getInputFileDir(const char* compiledFile);
void printVec(const std::vector<int>& vec);
void printVecChar(const std::vector<char>& vec);
void printVecPair(const std::vector<std::pair<int, int>>& vec);
void printVecVec(const std::vector<std::vector<int32_t>>& vec);
std::string removeLineBreaks(const std::string& str);
uint32_t count(const std::string& str, const char arg);
std::vector<std::vector<uint64_t>> parseUintData(const std::string& str);
std::vector<int> getDirections(const int len);
bool isCollinear(const int x1, const int y1, const int x2, const int y2,
                 const int x3, const int y3);
std::pair<int, int> getXY(const int pos, const size_t lineSize);
}  // namespace common
