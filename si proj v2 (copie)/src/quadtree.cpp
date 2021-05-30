
#include <vector>
#include "quadtree.h"
#include <iostream>

//vector<vector<int> > Map;

int height(int px, int py, int sizeX, int sizeY, vector<vector<int> > Map){
    // fonction calcule et renvoit la hauteur d'un point stocké dans le quadtree en accord avec la height map
    px += sizeX/2 ;
    py += sizeY/2 ;
    //std::cout << "px " << px << "py " << py << std::endl;
    //std::cout << "w " << Map.size() << "h " << py << std::endl;
    int test = Map[px][py];
    //std::cout << "b" << std::endl;
    return test ;
}

Point3D milieuPts(Point3D p1, Point3D p2 , int sizeX, int sizeY, vector<vector<int> > Map){
    // fonction calcule et renvoit les coordonnees du milieu de 2 points, sa hauteur est calculee en accord avec la height map
    Point3D p3;
    p3.x = (p1.x + p2.x)/2;
    p3.y = (p1.y + p2.y)/2;
    p3.z = height(p3.x, p3.y, sizeX, sizeY, Map);
    return p3;
}
