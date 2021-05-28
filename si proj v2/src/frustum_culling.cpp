
#include <string>

#include "frustum_culling.h"

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
    return out; 
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
    return out;
}

bool toDraw(triangle t, Camera c){
    bool out = true;
    if((outplan(t,c.planNear) == false && outplan(t,c.planFar) == false )||( outdroite(t,c.droiteG) == false && outdroite(t,c.droiteD) == false )){
        out = false;
    }
    return out;
}

bool LOD(Node* node, Camera c){
    Point3D p = milieuTPts(node->data->Cupleft, node->data->Cdownright);
    Vector3D v =  createVectorFromPoints(p, c.origin);
    int distance = norm(v);
    if (distance > 100-node->data->deep*5){
        return false;
    }
    else{
        return true;
    }
}