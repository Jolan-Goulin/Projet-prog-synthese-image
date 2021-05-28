#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vector3D, Point3D;

typedef struct droite {
    Vec3f Dir;
    Point3D p;
    Vec3f normale;
} droite;

typedef struct triangle {
    Point3D p1;
    Point3D p2;
    Point3D p3;
} triangle ;

typedef struct plan{
    Vec3f normale;
    Point3D p;
}plan;

typedef struct Camera {
    Point3D origin;
    droite droiteD;
    droite droiteG;
    plan planNear;
    plan planFar;
    int phi;
    int ro;
    int fov;
} Camera;



struct point{
    int x;
    int y;
};

void geometryCamera(Camera* c, int zFar, int zNear, int fov);
void checkNormals(Camera *c);
Point3D milieuPts(Point3D p1, Point3D p2);
Point3D createPoint(float x, float y, float z);
triangle createTriangle(Point3D p1, Point3D p2, Point3D p3);
Vector3D createVector(float x, float y, float z);
Vector3D createVectorFromPoints(Point3D p1, Point3D p2);
Vector3D regard(Camera c);
Vector3D produitVect(Vector3D v1, Vector3D v2);

Point3D pointPlusVector(Point3D p, Vector3D v);

Vector3D addVectors(Vector3D v1, Vector3D v2);
Vector3D subVectors(Vector3D v1, Vector3D v2);
Vector3D rotat(Vector3D v, int phi);

Vector3D multVector(Vector3D v, float a);
Vector3D divVector(Vector3D v, float a);

float dot(Vector3D v1, Vector3D v2);

float norm(Vector3D v);
Vector3D normalize(Vector3D v);

Vector3D reflect(Vector3D v, Vector3D n);

void printPoint3D(Point3D p);
void printVector3D(Vector3D v);

#endif