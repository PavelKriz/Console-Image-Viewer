#include <fstream>
#include <vector>

using namespace std;

class CFileSaver{
    string relativeFilePath_;
public:
    CFileSaver(string relativeFilepath);
    void safeFile(const vector<vector<char>> & toSafe);
};