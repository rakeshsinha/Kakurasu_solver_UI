#ifndef KAKURASU_H
#define KAKURASU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <set>
#include <list>
#include <QTextEdit>

using namespace std;
struct elementId { int indices[2];};
struct bits { bitset<9> e;};

inline bool operator<(const bits & e1, const bits& e2)
{
    bitset<9> tp1 = e1.e;
    bitset<9> tp2 = e2.e;
    return ((int)tp1.to_ulong() < (int)tp2.to_ulong());
}

inline bool operator<(const elementId& e1, const elementId& e2)
{
    if(e1.indices[0] == e2.indices[0])
        return e1.indices[1]<e2.indices[1];
    return e1.indices[0]<e2.indices[0];
}

struct fValue
{
    int rindex;
    int cindex;
    float scoreValue;
};
//October 19 - Easy 8 x 8

class Kakurasu
{
public:
    //Kakurasu(char*); //For UI
    Kakurasu(char*, QTextEdit*);
    void getAllCombinations();
    void printSolMatrix();
    int sizeOfTheProblem;
    int UnidentifiedVariables;
    bool **solMatrix;
    int *rowSum;
    int *columnSum;
    vector<vector<bitset<9>>*> *rows;
    vector<vector<bitset<9>>*> *columns;
    int *rowAssignmentCount;
    int *colAssignmentCount;
    int *rowSortedIndex;
    int *colSortedIndex;
    void sortIndex(int *, int *);
    int **checkMatrix;
    void ACAlgorithm(vector<vector<bitset<9>>*> *, vector<vector<bitset<9>>*> *, int **);
    set<elementId> pQueue;
    set<elementId> listOfVariables;
    int **fMatrix;
    int unidentifiedVariables(int **, int **);
    list<fValue> fValueList;
    void stm(list<fValue>& , int **, int **);
    float FValueCalculate(int, int, list<fValue>);
    list<elementId> elementsToCheck;
    list<int> col_position;
    list<int> row_position;
    set<bits> colVariableCombinations;
    set<bits> colVariableCombinations1;
    set<bits> rowVariableCombinations;
    set<bits> rowVariableCombinations1;
    void searchalgorithm(vector<vector<bitset<9>>*> *, vector<vector<bitset<9>>*> *, int **, int ** , list<fValue>);
    void printCheckMatrix(int **);
    void printfMatrix(int **);
    bool solutionFound;

    QTextEdit *qTextEdit; //For UI
};

#endif // KAKURASU_H
