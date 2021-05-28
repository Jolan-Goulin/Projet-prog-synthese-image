
#include <string>
#include <vector>

using namespace std;

struct tData{
    std::string fileName;
    int Xsize;
    int Ysize;
    int Zmin;
    int Zmax;
    int Znear;
    int Zfar;
    int fov;
    vector<vector<int> > Map;
};

