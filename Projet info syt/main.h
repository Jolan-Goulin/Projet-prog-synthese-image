#include <fstream>
#include <string>
#include <iostream>
using namespace std;

struct plan{
    
};

struct timacData{
    std::string fileName;
    int Xsize;
    int Ysize;
    int Zmin;
    int Zmax;
    plan Znear;
    plan Zfar;
    int fov;
};

