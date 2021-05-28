
#include <vector>
#include "quadtree.h"
vector<vector<int> > Map;


int height(int px, int py, int sizeX, int sizeY){
    px = px +sizeX/2;
    py = py +sizeY/2;
    int height = Map[px][py];
    return height;
}

Point3D milieuPts(Point3D p1, Point3D p2){
    Point3D p3;
    p3.x = (p1.x + p2.x)/2;
    p3.y = (p1.y + p2.y)/2;
    p3.z = height(p3.x, p3.y,sizeX,sizeY);
    return p3;
}
