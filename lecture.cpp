
#include <fstream>
#include <string>
#include <vector>
#include "lecture.h"
using namespace std;



vector<vector<int>> readhm(string(nomfichierhm))
{
    string const scores(nomfichierhm);
    ifstream monFlux2(nomfichierhm);
    if(monFlux2)
    {   
        string passestr; //pour passer la chaîne de caractère du début
        int xhm, yhm, valeur;
        int passe;       //pour passer le 255

        monFlux2 >> passestr;
        monFlux2 >> xhm; //Nombre de x
        monFlux2 >> yhm; //Nombre de y
        monFlux2 >> passe;

        vector<vector<int>> hm;


        for(int i1=0; i1 < yhm; i1++)
        {
            for(int i2=0; i2 < xhm; i2++)
            {
                monFlux2 >> valeur;
                hm[i1][i2]=valeur;
            }
        }
    }
    else
    {
        //cout << "ERREUR : Impossible d'ouvrir le fichier 2." << endl;
    }
    return hm;
}


void fetchData(tData* tdata) 
{
    string const scores("test.timac");

    ifstream monFlux("test.timac"); 

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
        //cout << "ERREUR : Impossible d'ouvrir le fichier." << endl;
    }  
}



