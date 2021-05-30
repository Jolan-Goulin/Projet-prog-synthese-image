#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lecture.h"
using namespace std;



vector<vector<int> > readhm(string nomfichierhm)
{
    // fonction qui lit la height map et renvoit un tableau contenant les hauteurs de ses points
    std::cout << "y " << std::endl;
    string const scores(nomfichierhm);
    ifstream monFlux2(nomfichierhm.c_str());
    if(monFlux2)
    {   
        string passestr; //pour passer la chaîne de caractère du début
        int xhm, yhm, valeur;
        int passe;       //pour passer le 255

        monFlux2 >> passestr;
        monFlux2 >> xhm; //Nombre de x
        monFlux2 >> yhm; //Nombre de y
        monFlux2 >> passe;

        std::cout << "x " << xhm << std::endl;
        std::cout << "y " << yhm << std::endl;

        vector<vector<int> > hm;

        hm.resize(xhm);
        for (int x = 0; x < xhm; ++x) 
        {
            hm[x].resize(yhm);
        }


        for(int y=0; y < yhm; y++)
        {
            for(int x=0; x < xhm; x++)
            {
                monFlux2 >> valeur;
                hm[x][y]=valeur;
            }
            //cout << "bla" << endl;
        }
        return hm;
    }
    else
    {
        cout << "ERREUR : Impossible d'ouvrir le fichier 2." << endl;
    };
}


void fetchData(tData* tdata) 
{
    // fonction qui lit le fichier .timac et stocke ses informations dans une structure tData
    string const scores("src/test.timac");

    ifstream monFlux("src/test.timac"); 
    
    if(monFlux)
    {   
        string nomfichier;
        monFlux >> nomfichier; 
        int Xsize, Ysize, zmin, zmax, znear, zfar, fov;
        monFlux >> Xsize; 
        monFlux >> Ysize;
        monFlux >> zmin; 
        monFlux >> zmax;
        monFlux >> znear; 
        monFlux >> zfar;
        monFlux >> fov;
        tdata->fileName = nomfichier;
        tdata->Xsize=Xsize;
        tdata->Ysize=Ysize;
        tdata->Zmin=zmin;
        tdata->Zmax=zmax;
        tdata->Znear=znear;
        tdata->Zfar=zfar;
        tdata->fov=fov;
        tdata->Map=readhm(nomfichier);
    }
   
    else //Affiche erreur sinon
    {
        cout << "ERREUR : Impossible d'ouvrir le fichier." << endl;
    }  
}



