
#include <string>

#include "frustum_culling.h"

using namespace std;

bool outplan(triangle t, plan p){
    // fonction qui determine si un point est du coté du plan opposé a celui ou est défini sa normale et renvoie true si il l'est et 
    // false sinon
    bool out = true;
    Vector3D vect  = createVectorFromPoints(t.p1, p.p);
    if (dot(vect, p.normale)>0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p2, p.p);
    if (dot(vect, p.normale)>0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p3, p.p);
    if (dot(vect, p.normale)>0){
        out= false;
    }
    return out; 
}

bool outdroite(triangle t, droite d){
    // fonction qui determine si un point est du coté d'une droite opposé a celui ou est défini sa normale et renvoie true si il l'est et 
    // false sinon, ON l'utilise dans un plan 2D qui ne prend pas en compte l'axe z
    bool out = true;
    Vector3D vect  = createVectorFromPoints(t.p1, d.p);
    if (dot(vect, d.normale)>0){
        out= false  ;
    }
    vect  = createVectorFromPoints(t.p2, d.p);
    if (dot(vect, d.normale)>0){
        out= false;
    }
    vect  = createVectorFromPoints(t.p3, d.p);
    if (dot(vect, d.normale)>0){
        out= false;
    }
    return out;
}

bool toDraw(triangle t, Camera c){
    // fonction qui determine si on doit dessiner ou non un triangle grace au frustum culling defini dans le sujet
    // elle revoit true si on doit dessiner le triangle et false sinon
    bool out = false;
    if((outplan(t,c.planNear) == false && outplan(t,c.planFar) == false )&&( outdroite(t,c.droiteG) == false && outdroite(t,c.droiteD) == false )){
        out = true;
    }
    return out;
}

bool LOD(Node* node, Camera c){
    // fonction qui determine si on doit dessiner la node ou affiner la precision en passant a ses enfants
    // elle revoit true si on doit dessiner la node et false si on passe aux enfants
    Point3D p = milieuTPts(node->data.Cupleft, node->data.Cdownright);
    Vector3D v =  createVectorFromPoints(p, c.origin);
    int distance = norm(v);
    if (distance < 400-node->data.deep*10){
        return false;
    }
    else{
        return true;
    }
}