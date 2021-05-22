#include <fstream>
#include <iostream>
#include <vector>
#include <geometry.h>

using namespace std;


struct point{
    int x;
    int y;
};

point milieuPts(point p1, point p2){
    point p3;
    p3.x = (p1.x + p2.x)/2;
    p3.y = (p1.y + p2.y)/2;
    return p3;
}

int height(point p, int mapX, int mapY, vector<vector<int>> Map){
    p.x = p.x +mapX/2;
    p.y = p.y +mapY/2;
    int height = Map[p.x][p.y];
    return height;
}

struct dataNode {
    int mapX;
    int mapY;
    Point3D Cupleft;
    Point3D Cdownleft;
    Point3D Cupright;
    Point3D Cdownright;
    int deep;
};

struct Node
{   
    //données
    dataNode* data;

    // nodes enfants
    Node* upleft;
    Node* downleft;
    Node* upright;
    Node* downright;
    
    bool leaf;

    triangle triangle1(){

    }
    bool isLeaf(){
        return this->leaf;
    }
    //fonction qui dertermine si une node est leaf et la divise en 4 nodes enfants sinon
    void initMapNode(Point3D Dupleft,Point3D Ddownleft,Point3D Dupright,Point3D Ddownright,int deep){
        this->data->Cdownleft = Ddownleft;
        this->data->Cdownright =Ddownright;
        this->data->Cupleft = Dupleft;
        this->data->Cupright = Dupright;



        this->data->mapX = this->data->Cupright.x-this->data->Cupleft.x;
        this->data->mapY = this->data->Cupright.y-this->data->Cdownright.y;
        if (this->data->mapX<=2 || this->data->mapY<=2){
            this->leaf = true;
            this->upleft = NULL;
            this->downleft = NULL;
            this->upright = NULL;
            this->downright = NULL;
        }
        else{
            this->upleft = new Node;
            this->downleft = new Node;
            this->upright = new Node;
            this->downright = new Node;
            this->upleft->  initMapNode(this->data->Cupleft,   
                milieuPts(this->data->Cupleft, 
                this->data->Cdownleft),
                milieuPts(this->data->Cupleft, this->data->Cupright),
                milieuPts(this->data->Cupleft, this->data->Cdownright), 
                deep+1);
            this->downleft->initMapNode(milieuPts(this->data->Cupleft, 
                this->data->Cdownleft),
                this->data->Cdownleft,
                milieuPts(this->data->Cdownleft, 
                this->data->Cupright),
                milieuPts(this->data->Cdownleft, 
                this->data->Cdownright), 
                deep+1);
            this->upright-> initMapNode(milieuPts(this->data->Cupleft, this->data->Cupright),milieuPts(this->data->Cdownleft,this->data->Cupright),this->data->Cupright,milieuPts(this->data->Cupright, this->data->Cdownright), deep+1);
            this->downright->initMapNode(milieuPts(this->data->Cupleft, this->data->Cdownright),milieuPts(this->data->Cdownleft, this->data->Cdownright),milieuPts(this->data->Cupright, this->data->Cdownright),this->data->Cdownright, deep+1);
        }
    }
};


struct quadtree 
{    
    int mapX;
    int mapY;
    vector<vector<int>> Map;

    Point3D Cupleft;
    Point3D Cdownleft;
    Point3D Cupright;
    Point3D Cdownright;

    Node* first;
    
    void initMap (vector<vector<int>> map, int mapX, int mapY){
        this->mapX = mapX;
        this->mapY = mapY;
        this->Map = map;
        this->Cupleft.x = -mapX/2;
        this->Cupleft.y = mapY-mapY/2;
        this->Cupleft.z = Map[this->Cupleft.x][this->Cupleft.y];
        this->Cdownleft.x = -mapX/2;
        this->Cdownleft.y = -mapY/2;
        this->Cdownleft.z = Map[this->Cdownleft.x][this->Cdownleft.y];
        this->Cupright.x = mapX-mapX/2;
        this->Cupright.y = mapY-mapY/2;
        this->Cupright.z = Map[this->Cupright.x][this->Cupright.y];
        this->Cdownright.x = mapX-mapX/2;
        this->Cdownright.y = -mapY/2;
        this->Cdownright.z = Map[this->Cdownright.x][this->Cdownright.y];
        this->first->initMapNode(this->Cupleft,this->Cdownleft,this->Cupright,this->Cdownright,0);
    }

};

