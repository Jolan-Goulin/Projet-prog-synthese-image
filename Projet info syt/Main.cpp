#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <geometry.h>
#include <frustum_culling.cpp>
#include <quadtree.h>
#include <colors.h>
#include <colors.cpp>

/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "TD03 Minimal";
Camera cam;
quadtree QT;
Point3D sun = {0,0,800};
int sunAngle = 0;


bool lines = false;

void quadtreeTravelDraw(Camera c, Node* QT, Point3D sun){
    triangle t1 = createTriangle( QT->data->Cdownleft, QT->data->Cupleft, QT->data->Cupright );
    triangle t2 = createTriangle( QT->data->Cupright, QT->data->Cdownright, QT->data->Cdownleft);
    if(QT->isLeaf() || LOD(QT, c)){
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
        if (toDraw(t1, c) && toDraw(t2, c)){
            quadtreeTravelDraw(c, QT->downleft, sun);
            quadtreeTravelDraw(c, QT->upleft, sun);
            quadtreeTravelDraw(c, QT->downright, sun);
            quadtreeTravelDraw(c, QT->upright, sun);
        }
        else if (toDraw(t1, c)){
            quadtreeTravelDraw(c, QT->downleft, sun);
            quadtreeTravelDraw(c, QT->upleft, sun);
            quadtreeTravelDraw(c, QT->upright, sun);
        }
        else if (toDraw(t2, c)){
            quadtreeTravelDraw(c, QT->downleft, sun);
            quadtreeTravelDraw(c, QT->downright, sun);
            quadtreeTravelDraw(c, QT->upright, sun);
        }
    }
}

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 150.;

/* Subdivisions cercle */
static const int CIRCLE_SUBDIVS = 2<<5;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void onWindowResized(unsigned int width, unsigned int height)
{ 
    float aspectRatio = width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( aspectRatio > 1) 
    {
        gluOrtho2D(
        -GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio, 
        -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
    }
    else
    {
        gluOrtho2D(
        -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
        -GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio);
    }
}


/* Objets cannoniques */
void drawTriangle(triangle t, Point3D sun) 
{
    Col3f triColor = createColor(200/255, 25/255, 25/255);
    Col3f sunColor = createColor(0.90, 0.41, 0.18);
    Col3f final = multColors(triColor, sunColor);

    Point3D center;
    center.x = t.p1.x/3 + t.p2.x/3 + t.p3.x/3;
    center.y = t.p1.y/3 + t.p2.x/3 + t.p3.y/3;
    center.z = t.p1.z/3 + t.p2.z/3 + t.p3.z/3;
    Vector3D IL = createVectorFromPoints(sun, center);
    Vector3D N = produitVect(createVectorFromPoints(t.p1, t.p2), createVectorFromPoints(t.p1, t.p3));
    final = multColor(final, dot(N, normalize(IL))/pow(norm(IL),2));


    glBegin(GL_TRIANGLES);

    glColor3f(final.r, final.g, final.b);
    glVertex3f( t.p1.x, t.p1.y, t.p1.z);
    glVertex3f( t.p2.x, t.p2.y, t.p2.z);
    glVertex3f( t.p3.x, t.p3.y, t.p3.z);

    glEnd();


}

void drawLinesTriangle(triangle t) 
{
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR,currentColor);

    glBegin(GL_LINES);

    glColor3f(1., 0., 0.);
    glVertex3f( t.p1.x, t.p1.y, t.p1.z);
    glVertex3f( t.p2.x, t.p2.y, t.p2.z);
    glVertex3f( t.p3.x, t.p3.y, t.p3.z);

    glEnd();

    glColor3fv(currentColor);
}



int main(int argc, char** argv) 
{
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

    int Alp = 1;
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
        glTranslatef(-cam.origin.x, -cam.origin.y, -cam.origin.z);
        glRotatef(cam.phi,0,0,1);
        glRotatef(cam.ro,cos(cam.phi),sin(cam.phi),0);

        




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
		
			if(	e.type == SDL_KEYDOWN 
				&& (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
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

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;
                
                /* mouvement souris (déplacement)*/
                case SDL_MOUSEMOTION:
                    cam.origin.x += e.motion.xrel;
                    cam.origin.y -= e.motion.yrel;
                    break;

                /* Touche clavier (rotation)*/
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                        if (e.key.keysym.sym == 'z'){
                            cam.ro++;
                        }
                        if (e.key.keysym.sym == 's'){
                            cam.ro--;
                        }
                        if (e.key.keysym.sym == 'd'){
                            cam.phi++;
                        }
                        if (e.key.keysym.sym == 'q'){
                            cam.phi--;
                        }
                        if (e.key.keysym.sym == 'a'){
                            lines = !lines;
                        }
                        if (e.key.keysym.sym == 'e'){
                            sunAngle++;
                            sun.x = sin(sunAngle);
                            sun.z = cos(sunAngle);
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
