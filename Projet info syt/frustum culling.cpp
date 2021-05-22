#include <fstream>
#include <iostream>
#include <string>
#include<geometry.h>

using namespace std;

bool outplan(triangle t, plan p){
    bool out = true;
    Vector3D vect  = createVectorFromPoints(t.p1, p.p);
    if (dot(vect, p.normale)<0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p2, p.p);
    if (dot(vect, p.normale)<0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p3, p.p);
    if (dot(vect, p.normale)<0){
        out= false;
    }
}

bool outdroite(triangle t, droite d){
    bool out = true;
    Vector3D vect  = createVectorFromPoints(t.p1, d.p);
    if (dot(vect, d.normale)<0){
        out= false  ;
    }
    vect  = createVectorFromPoints(t.p2, d.p);
    if (dot(vect, d.normale)<0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p3, d.p);
    if (dot(vect, d.normale)<0){
        out= false;
    }
}

bool toDraw(triangle t, Camera c){
    bool out = true;
    if(outplan(t,c.planNear) == false && outplan(t,c.planFar) == false || outdroite(t,c.droiteG) == false && outdroite(t,c.droiteD) == false ){
        out == false;
    }
    return out;
}

