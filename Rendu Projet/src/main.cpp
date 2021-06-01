#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include "frustum_culling.h"
#include "colors.h"
#include "lecture.h"




/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "Wow ca marche !";
Camera cam;
quadtree QT;
Point3D sun = createPoint(0,0,-800);
int sunAngle = 0;
tData data;
bool lines = false;
bool fc = false;
int lod = 150;
float taille = 25.0;

void drawTriangle(triangle t, Point3D sun, Col3f triColor = createColor(200.0/255, 25.0/255, 25.0/255)) 
{
    // fonction dessine un triangle passé en argument dans une couleur calculee par le modele de Lambert
    Col3f sunColor = createColor(0.90, 0.41, 0.18);
    Col3f final = multColors(triColor, sunColor);
    Point3D center;
    center.x = t.p1.x/3 + t.p2.x/3 + t.p3.x/3;
    center.y = t.p1.y/3 + t.p2.x/3 + t.p3.y/3;
    center.z = t.p1.z/3 + t.p2.z/3 + t.p3.z/3;
    Vector3D IL = createVectorFromPoints(sun, center);
    Vector3D N = produitVect(createVectorFromPoints(t.p1, t.p2), createVectorFromPoints(t.p1, t.p3));
    final = multColor(final, 0.1f + 0.9f * max(-dot(normalize(N), normalize(IL)), 0.f));

    glPushMatrix();
    glBegin(GL_TRIANGLES);

        glColor3f(final.r, final.g, final.b);
        //glColor3f(1.0, 0.5,0.5);
        glVertex3f( -float(t.p1.x), -float(t.p1.y), float(t.p1.z));
        glVertex3f( -float(t.p2.x), -float(t.p2.y), float(t.p2.z));
        glVertex3f( -float(t.p3.x), -float(t.p3.y), float(t.p3.z));

    glEnd();
    glPopMatrix();

}

void drawLinesTriangle(triangle t, Col3f triColor = createColor(200.0/255, 25.0/255, 25.0/255)) 
{
    // fonction dessine le contour d'un triangle passé en argument

    glBegin(GL_LINES);

    glColor3f(triColor.r, triColor.g, triColor.b);
        glVertex3f( -float(t.p1.x), -float(t.p1.y), float(t.p1.z));
        glVertex3f( -float(t.p2.x), -float(t.p2.y), float(t.p2.z));
        glVertex3f( -float(t.p2.x), -float(t.p2.y), float(t.p2.z));
        glVertex3f( -float(t.p3.x), -float(t.p3.y), float(t.p3.z));
        glVertex3f( -float(t.p3.x), -float(t.p3.y), float(t.p3.z));
        glVertex3f( -float(t.p1.x), -float(t.p1.y), float(t.p1.z));
    glEnd();

}

void quadtreeTravelDraw(Camera c, Node* Qt, Point3D sun){
    // fonction prend en argument la premiere node d'un quadtree et dessine la height map qu'elle contient
    // elle utilise les fonctions de Level Of Detail (LOD) et de Frustum Culling (toDraw)
    triangle t1 = createTriangle( Qt->data.Cdownleft, Qt->data.Cupleft, Qt->data.Cupright ); 
    triangle t2 = createTriangle( Qt->data.Cupright, Qt->data.Cdownright, Qt->data.Cdownleft);
    if(Qt->isLeaf()==true){
        if(lines){
            drawLinesTriangle(t1, createColor(25/255.0, 200/255.0, 25/255.0));
            drawLinesTriangle(t2, createColor(25/255.0, 200/255.0, 25/255.0));
        }
        else{
            drawTriangle(t1, sun);
            drawTriangle(t2, sun);
        }
    }
    else if(LOD(Qt, c, lod)){
        if(lines){
            drawLinesTriangle(t1);
            drawLinesTriangle(t2);
        }
        else{
            drawTriangle(t1, sun);
            drawTriangle(t2, sun);
        }
    }
    else{
        if(fc == true){
            if (toDraw(t1, c) && toDraw(t2, c)){
                quadtreeTravelDraw(c, Qt->downleft, sun);
                quadtreeTravelDraw(c, Qt->upleft, sun);
                quadtreeTravelDraw(c, Qt->downright, sun);
                quadtreeTravelDraw(c, Qt->upright, sun);
            }
            else if (toDraw(t1, c)){
                quadtreeTravelDraw(c, Qt->downleft, sun);
                quadtreeTravelDraw(c, Qt->upleft, sun);
                quadtreeTravelDraw(c, Qt->upright, sun);
            }
            else if (toDraw(t2, c)){
                quadtreeTravelDraw(c, Qt->downleft, sun);
                quadtreeTravelDraw(c, Qt->downright, sun);
                quadtreeTravelDraw(c, Qt->upright, sun);
            }
        }
    
        else{
            quadtreeTravelDraw(c, Qt->downleft, sun);
            quadtreeTravelDraw(c, Qt->upleft, sun);
            quadtreeTravelDraw(c, Qt->downright, sun);
            quadtreeTravelDraw(c, Qt->upright, sun);
        }
    }
}

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 150.;


/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void onWindowResized(unsigned int width, unsigned int height)
{ 
    float aspectRatio = width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fov, aspectRatio, data.Znear, data.Zfar);
}


/* Objets cannoniques */



int main(int argc, char** argv) 
{   
    fetchData(&data);

    //vector<vector<int> > Map = data->Map;
    std::cout << data.Map.size() << std::endl;
    QT.initMap (data.Map, data.Xsize, data.Ysize);
    cam.fov = data.fov;
    cam.origin.x = 0;
    cam.origin.y = 0;
    cam.origin.z = -20;
    //* Initialisation de la SDL */

    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        const char* error = SDL_GetError();
        fprintf(
            stderr, 
            "Erreur lors de l'intialisation de la SDL : %s\n", error);

        SDL_Quit();
        return EXIT_FAILURE;
    }
	
    /* Ouverture d'une fenetre et creation d'un contexte OpenGL */

    SDL_Window* window;
    {
        window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(NULL == window) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation de la fenetre : %s\n", error);

            SDL_Quit();
            return EXIT_FAILURE;
        }
    }
    
    SDL_GLContext context;
    {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        context = SDL_GL_CreateContext(window);
    
        if(NULL == context) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation du contexte OpenGL : %s\n", error);

            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }    
  
    onWindowResized(WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Boucle principale */
    int loop = 1;

    while(loop) 
    {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        geometryCamera(&cam ,  data.Zfar,  data.Znear,  cam.fov);
        
        glPushMatrix();
        
        //glBegin(GL_TRIANGLES);
            
            
            
            glRotatef(cam.ro,0,1,0); 
            glRotatef(cam.phi,1,0,0);
            glTranslatef(cam.origin.x, cam.origin.y, cam.origin.z);
            glScalef(0.5,0.5,1.0/taille);
            quadtreeTravelDraw(cam, QT.first, sun); 
             
        glPopMatrix();
        




        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapWindow(window);
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            /* L'utilisateur ferme la fenetre : */
			if(e.type == SDL_QUIT) 
			{
				loop = 0;
				break;
			}
	
            
            switch(e.type) 
            {
                case SDL_WINDOWEVENT:
                    switch (e.window.event) 
                    {
                        /* Redimensionnement fenetre */
                        case SDL_WINDOWEVENT_RESIZED:
                            onWindowResized(e.window.data1, e.window.data2);                
                            break;

                        default:
                            break; 
                    }
                    break;
                
                /* mouvement souris (déplacement)*/
                /*case SDL_MOUSEMOTION:
                    cam.origin.x += e.motion.xrel;
                    cam.origin.y -= e.motion.yrel;
                    break;*/

                /* Touche clavier (rotation)*/
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                        if (e.key.keysym.sym == 'z'){
                            cam.ro+=5;
                        }
                        if (e.key.keysym.sym == 's'){
                            cam.ro-=5;
                        }
                        if (e.key.keysym.sym == 'd'){
                            cam.phi+=5;
                        }
                        if (e.key.keysym.sym == 'w'){
                            cam.phi-=5;
                        }
                        if (e.key.keysym.sym == 'a'){
                            lines = !lines;
                        }
                        if (e.key.keysym.sym == 'f'){
                            fc = !fc;
                        }
                        if (e.key.keysym.sym == 't'){
                            taille+=5;
                        }
                        if (e.key.keysym.sym == 'g'){
                            taille=max(1.0,taille-5.0);
                        }
                        if (e.key.keysym.sym == 'n'){
                            lod+=5;
                        }
                        if (e.key.keysym.sym == 'b'){
                            lod=max(1,lod-5);
                        }
                        if (e.key.keysym.sym == 'e'){
                            sunAngle+=5*360.0/2*M_PI;
                            sun.x = 800*sin(sunAngle);
                            sun.z = 800*cos(sunAngle);
                        }
                        if (e.key.keysym.sym == 'i'){
                            cam.origin.x += 1;
                        }
                        if (e.key.keysym.sym == 'k'){
                            cam.origin.x -= 1;
                        }
                        if (e.key.keysym.sym == 'j'){
                            cam.origin.y -= 1;
                        }
                        if (e.key.keysym.sym == 'l'){
                            cam.origin.y += 1;
                        }
                        if (e.key.keysym.sym == 'p'){
                            cam.origin.z += 0.2;
                        }
                        if (e.key.keysym.sym == 'm'){
                            cam.origin.z -= 0.2;
                        }
                        break;


                default:
                    break;
            }
        }

        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associees a la SDL */ 
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}