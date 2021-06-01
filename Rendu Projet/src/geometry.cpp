#include <math.h>
#include <stdio.h>
#include "geometry.h"
using namespace std;

// fonctions permetant des opérations sur les vecteurs, les points et des opérations géometriques lies a la camera

Vector3D rotatz(Vector3D v, int phi){
    v.x = cos(phi)*v.x - sin(phi)*v.y;
    v.y = sin(phi)*v.x + cos(phi)*v.y;
    return v;
}

Vector3D regard(Camera c){
    // fonction qui calcule et renvoit le regard de la camera
    Point3D pv0 = createPoint (0,0,0);
    Point3D pv1 = createPoint (cos(c.phi),sin(c.phi),sin(c.ro));
    Vec3f regard = createVectorFromPoints( pv0,  pv1);
    return regard;
}


void checkNormals(Camera* c){
    // fonction qui verifie si les normales des plans et droites liés a la camera sont en accord avec leur utilisation dans le frustum culling
    // et les inverse sinon
    Vector3D r = regard(*c);
    if (dot(c->planFar.normale, r)<0){
        c->planFar.normale = multVector(c->planFar.normale, -1);
    }
    if (dot(c->planNear.normale, r)>0){
        c->planNear.normale = multVector(c->planNear.normale, -1); 
    }
    if (dot(c->droiteD.normale, r)>0){
        c->droiteD.normale = multVector(c->droiteD.normale, -1);
    }
    if (dot(c->droiteG.normale, r)>0){
        c->droiteG.normale = multVector(c->droiteG.normale, -1);
    }
}

void geometryCamera(Camera* c, int zFar, int zNear, int fov){
    // fonction qui calcule les plan et droites lies a la camera
    Vec3f r = regard(*c);
    c->planFar.normale = r;
    c->planFar.p = multVector(r, zFar);

    c->planNear.normale = r;
    c->planNear.p = multVector(r, zNear);

    c->droiteD.p = c->origin;
    c->droiteD.Dir = rotatz(r, +c->fov/2);
    c->droiteD.normale = createVector(c->droiteD.Dir.x, -c->droiteD.Dir.y, 0);

    c->droiteG.p = c->origin;
    c->droiteG.Dir = rotatz(r, -c->fov/2);
    c->droiteG.normale = createVector(c->droiteG.Dir.x, -c->droiteG.Dir.y, 0);

    checkNormals(c);

}

triangle createTriangle(Point3D p1, Point3D p2, Point3D p3){
    triangle t;
    t.p1 =p1;
    t.p2 =p2;
    t.p3 =p3;
    return t;
}

Point3D milieuTPts(Point3D p1, Point3D p2){
    Point3D p;
    p.x = p1.x/2 + p2.x/2;
    p.y = p1.y/2 + p2.y/2;
    p.z = p1.z/2 + p2.z/2;
    return p;
}

Vector3D produitVect(Vector3D v1, Vector3D v2){
    Vector3D v;
    v.x = v1.y*v2.z - v1.z*v2.y;
    v.y = v1.z*v2.x - v1.x*v2.z;
    v.z = v1.x*v2.y - v1.y*v2.x;
    return v;
}

Point3D createPoint(float x, float y, float z) 
{
    Point3D p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

Vector3D createVector(float x, float y, float z)
{
    Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

Vector3D createVectorFromPoints(Point3D p1, Point3D p2)
{
    Vector3D v;
    v.x = p2.x-p1.x;
    v.y = p2.y-p1.y;
    v.z = p2.z-p1.z;
    return v;
}




Vector3D addVectors(Vector3D v1, Vector3D v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

Vector3D subVectors(Vector3D v1, Vector3D v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

Vector3D multVector(Vector3D v, float a)
{
    if (a == 0.) {
        v.x = 0.;
        v.y = 0.;
        v.z = 0.;
        return v; 
    }

    v.x *= a;
    v.y *= a;
    v.z *= a;
    return v;
}

Vector3D divVector(Vector3D v, float a)
{
    if (a == 0.) 
        return v;
    v.x /= a;
    v.y /= a;
    v.z /= a;
    return v;
}

float dot(Vector3D v1, Vector3D v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float norm(Vector3D v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D normalize(Vector3D v)
{
    float n = norm(v);
    if (n == 0.)
        return v;
    v.x /= n;
    v.y /= n;
    v.z /= n;
    return v;
}

Vector3D reflect(Vector3D v, Vector3D n)
{
    return subVectors(v, multVector(n, 2. * dot(v, n)));
}


