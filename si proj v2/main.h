#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <geometry.h>
#include <quadtree.h>
#include <colors.h>
using namespace std;

struct tData{
    std::string fileName;
    int Xsize;
    int Ysize;
    int Zmin = 0;
    int Zmax;
    int Znear;
    int Zfar;
    int fov;
    vector<vector<int>> Map;
};

void quadtreeTravelDraw(Camera c, Node* Qt, Point3D sun);
void drawTriangle(triangle t, Point3D sun, Col3f triColor = createColor(200/255, 25/255, 25/255)) ;
void drawLinesTriangle(triangle t);