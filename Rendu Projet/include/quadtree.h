#include <vector>
#include "geometry.h"
#include <iostream>

using namespace std;


Point3D milieuPts(Point3D p1, Point3D p2,int sizeX, int sizeY, vector<vector<int> > Map);
int height(int px, int py, int sizeX, int sizeY, vector<vector<int> > Map);

// structure servant a stocker les informations contenues dans une node
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
    dataNode data;

    // nodes enfants
    Node* upleft;
    Node* downleft;
    Node* upright;
    Node* downright;
    
    bool leaf;


    bool isLeaf(){
        return this->leaf;
    }
    //fonction qui dertermine si une node est leaf et la divise en 4 nodes enfants sinon
    void initMapNode(Point3D Dupleft,Point3D Ddownleft,Point3D Dupright,Point3D Ddownright,int deep, int sizeX, int sizeY, vector<vector<int> > map){
        this->data.Cdownleft = Ddownleft;
        this->data.Cdownleft.z = height( this->data.Cdownleft.x, this->data.Cdownleft.y, sizeX, sizeY, map);
        this->data.Cdownright =Ddownright;
        this->data.Cdownright.z = height( this->data.Cdownright.x, this->data.Cdownright.y, sizeX, sizeY, map);
        this->data.Cupleft = Dupleft;
        this->data.Cupleft.z = height( this->data.Cupleft.x, this->data.Cupleft.y, sizeX, sizeY, map);
        this->data.Cupright = Dupright;
        this->data.Cupright.z = height(this->data.Cupright.x, this->data.Cupright.y, sizeX, sizeY, map);
        this->data.deep = deep;


        this->data.mapX = this->data.Cupright.x-this->data.Cupleft.x;
        this->data.mapY = this->data.Cupright.y-this->data.Cdownright.y;
        if (this->data.mapX<=2 || this->data.mapY<=2){
            this->leaf = true;
            this->upleft = NULL;
            this->downleft = NULL;
            this->upright = NULL;
            this->downright = NULL;
        }
    
        else{
            this->leaf = false;
            this->upleft = new Node;
            this->downleft = new Node;
            this->upright = new Node;
            this->downright = new Node;
            this->upleft->  initMapNode(this->data.Cupleft,   
                milieuPts(this->data.Cupleft, this->data.Cdownleft, sizeX, sizeY, map),
                milieuPts(this->data.Cupleft, this->data.Cupright, sizeX, sizeY, map),
                milieuPts(this->data.Cupleft, this->data.Cdownright, sizeX, sizeY, map), 
                deep+1,  sizeX,  sizeY, map);
            this->downleft->initMapNode(milieuPts(this->data.Cupleft, this->data.Cdownleft, sizeX, sizeY, map),
                this->data.Cdownleft,
                milieuPts(this->data.Cdownleft, this->data.Cupright, sizeX, sizeY, map),
                milieuPts(this->data.Cdownleft, this->data.Cdownright, sizeX, sizeY, map), 
                deep+1,  sizeX,  sizeY, map);
            this->upright-> initMapNode(milieuPts(this->data.Cupleft, this->data.Cupright, sizeX, sizeY, map),
                milieuPts(this->data.Cdownleft,this->data.Cupright, sizeX, sizeY, map),
                this->data.Cupright,
                milieuPts(this->data.Cupright, this->data.Cdownright, sizeX, sizeY, map), 
                deep+1,  sizeX,  sizeY, map);
            this->downright->initMapNode(milieuPts(this->data.Cupleft, this->data.Cdownright, sizeX, sizeY, map),
                milieuPts(this->data.Cdownleft, this->data.Cdownright, sizeX, sizeY, map),
                milieuPts(this->data.Cupright, this->data.Cdownright, sizeX, sizeY, map),
                this->data.Cdownright, 
                deep+1,  sizeX,  sizeY, map);
        }
    }
};


struct quadtree 
{    
    //vector<vector<int> > Map;
    int mapX;
    int mapY;

    Point3D Cupleft;
    Point3D Cdownleft;
    Point3D Cupright;
    Point3D Cdownright;

    Node* first;
    
    void initMap (vector<vector<int> > Map, int mapX, int mapY){
        //fonction qui initialise le quadtree a partir de la height map et de ses dimensions 
        this->first = new Node;
        this->mapX = mapX;
        this->mapY = mapY;
        //this->Map = map;
        this->Cupleft.x = -mapX/2;
        this->Cupleft.y = mapY-mapY/2-1;
        this->Cupleft.z = height(this->Cupleft.x, this->Cupleft.y, mapX, mapY, Map);
        this->Cdownleft.x = -mapX/2;
        this->Cdownleft.y = -mapY/2;
        this->Cdownleft.z = height(this->Cdownleft.x, this->Cdownleft.y, mapX, mapY, Map);
        this->Cupright.x = mapX-mapX/2-1;
        this->Cupright.y = mapY-mapY/2-1;
        this->Cupright.z = height(this->Cupright.x, this->Cupright.y, mapX, mapY, Map);
        this->Cdownright.x = mapX-mapX/2-1;
        this->Cdownright.y = -mapY/2;
        this->Cdownright.z = height(this->Cdownright.x, this->Cdownright.y, mapX, mapY, Map);
        this->first->initMapNode(this->Cupleft,this->Cdownleft,this->Cupright,this->Cdownright,0, this->mapX, this->mapY, Map);
    }

};


