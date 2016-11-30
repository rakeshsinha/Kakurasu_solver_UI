#include <QApplication>

#include "mainwindow.h"
#include "kakurasu.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    //-----------------------------------------------------------------------------------



    //Kakurasu *kakurasu;
    //if(argc == 2)
    //{
    //    kakurasu = new Kakurasu(argv[1]);
    //}
    //else
    //{
    //    cout<<"\n Continuing with UI";
    //}

    //kakurasu->printSolMatrix();
    //kakurasu->getAllCombinations();
    //kakurasu->ACAlgorithm((kakurasu->rows), (kakurasu->columns), kakurasu->checkMatrix);
    //kakurasu -> UnidentifiedVariables = kakurasu->unidentifiedVariables(kakurasu->checkMatrix, kakurasu->fMatrix);
    //cout<<"\n\n In main : number of Unidentified Variables are : "<<kakurasu -> UnidentifiedVariables <<"\n\n";
    //if(kakurasu->UnidentifiedVariables == 0)
    //{
    //    cout<<"\n Problem is Solved Using AC Algorithm";
    //}
    //else
    //{
    //    kakurasu->stm(kakurasu->fValueList, kakurasu->checkMatrix, kakurasu->fMatrix);
    //    fflush(stdout);
    //    kakurasu->searchalgorithm(kakurasu->rows, kakurasu->columns, kakurasu->checkMatrix, kakurasu->fMatrix, kakurasu->fValueList);
    //}

    //-----------------------------------------------------------------------------------

    return a.exec();
}
