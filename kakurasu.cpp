#include <fstream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <iterator>

#include "kakurasu.h"

Kakurasu::Kakurasu(char *fileName, QTextEdit *tempQtextEdit)
{
    this->qTextEdit = tempQtextEdit;
	solutionFound = false;
    string line, token;
    stringstream ss;
    UnidentifiedVariables = 0;
    ifstream in(fileName);
    if (in.is_open())
    {
        getline(in, line);
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        sizeOfTheProblem = atoi(line.c_str());

        solMatrix = new bool*[sizeOfTheProblem + 1];
        for(int i = 1; i <= sizeOfTheProblem; i++)
            solMatrix[i] = new bool[sizeOfTheProblem + 1];

        for(int i = 1; i <= sizeOfTheProblem; i++)
            for(int j = 1; j <= sizeOfTheProblem; j++)
                solMatrix[i][j] = false;

        rowSum = new int[sizeOfTheProblem + 1];
        columnSum = new int[sizeOfTheProblem + 1];
        rowAssignmentCount = new int[sizeOfTheProblem+1];
        colAssignmentCount = new int[sizeOfTheProblem+1];
        rowSortedIndex = new int[sizeOfTheProblem+1];
        colSortedIndex = new int[sizeOfTheProblem+1];
        for(int i = 1; i <= sizeOfTheProblem; i++)
		{
        	rowAssignmentCount[i] = 0;
        	colAssignmentCount[i] = 0;
        	rowSortedIndex[i] = i;
        	colSortedIndex[i] = i;

		}
        checkMatrix = new int* [sizeOfTheProblem+1];
        fMatrix = new int* [sizeOfTheProblem+1];
        for(int i = 0; i<=sizeOfTheProblem; i++)
        {
        	checkMatrix[i] = new int[sizeOfTheProblem+1];
        	fMatrix[i] = new int[sizeOfTheProblem+1];
        }
        for(int i = 0; i<=sizeOfTheProblem; i++)
        {
        	for(int j = 0; j<=sizeOfTheProblem; j++)
        	{
        		checkMatrix[i][j] = -1;
        	}
        }
        int count = 0;
        while(getline(in, line))
        {
            if(count > sizeOfTheProblem)
            {
                qTextEdit->insertPlainText("\nArguments more than the size of the matrix");
                break;
            }
            else
            {
                ss.str("");
                ss.clear();
                ss.str(line);
                string str[2];
                int i=0;
                while(getline(ss, token, ','))
                    str[i++] = token;

                if(atoi(str[0].c_str()) == -1 && atoi(str[1].c_str()) != -1)
                {
                	rowSum[++count] = -1;
                	columnSum[count] = atoi(str[1].c_str());
                }
                else if(atoi(str[1].c_str()) == -1 && atoi(str[0].c_str()) != -1)
				{
                	rowSum[++count] = atoi(str[0].c_str());
                	columnSum[count] = -1;
				}
                else if(atoi(str[1].c_str()) == -1 && atoi(str[0].c_str()) == -1)
                {
                	rowSum[++count] = -1;
                	columnSum[count] = -1;
                }
                else
                {
                	rowSum[++count] = atoi(str[0].c_str());
                	columnSum[count] = atoi(str[1].c_str());
                }
            }
        }
    }
    in.close();

    rows = new vector<vector<bitset<9>>*>;
    columns = new vector<vector<bitset<9>>*>;
    for(int i = 0; i<=sizeOfTheProblem - 1; i++)
    {
        rows->push_back((new vector<bitset<9>>));
        columns->push_back((new vector<bitset<9>>));
    }
}


void Kakurasu::printSolMatrix()
{
    for(int i = 1; i <= sizeOfTheProblem; i++)
    {
        qTextEdit->insertPlainText("\n");
        for(int j = 1; j <= sizeOfTheProblem; j++)
            qTextEdit->insertPlainText("\t" + QString::number(solMatrix[i][j]));
        qTextEdit->insertPlainText("\t" + QString::number(rowSum[i]));
    }
    qTextEdit->insertPlainText("\n");
    for(int i = 1; i <= sizeOfTheProblem; i++)
        qTextEdit->insertPlainText("\t" + QString::number(columnSum[i]));
}


void Kakurasu::getAllCombinations()
{
    int sum = 0;
    for(long int i = 1; i<=(pow(2,sizeOfTheProblem) - 1); i++)
    {
        sum = 0;
        bitset<9> set(i);

        for(int j = 1; j<=sizeOfTheProblem; j++)
        {
            sum = sum + set[j - 1] * j;
        }
        for(int j = 1; j<=sizeOfTheProblem; j++)
        {
            if(sum == rowSum[j])
            {
                rows->at(j - 1)->push_back(set);
                rowAssignmentCount[j]+=1;
            }
            if(sum == columnSum[j])
            {
                columns->at(j - 1)->push_back(set);
                colAssignmentCount[j]+=1;
            }
            if(rowSum[j] == -1)
            {
            	rows->at(j - 1)->push_back(set);
            	rowAssignmentCount[j]+=1;
            }
            if(columnSum[j] == -1)
			{
            	columns->at(j - 1)->push_back(set);
            	colAssignmentCount[j]+=1;
			}
        }
    }
    qTextEdit->insertPlainText("\n-------------- Initial Combinations:----------------- \n");
    qTextEdit->insertPlainText("\nFor all rows\n");
    int count = 0;
    for(vector<vector<bitset<9>>*>::iterator it = rows->begin() ; it != rows->end(); ++it)
    {
        ++count;
        qTextEdit->insertPlainText("\nFor row : " + QString::number(count) + "\n");
        for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
    }

    qTextEdit->insertPlainText("\nFor all columns\n");
    count = 0;
    for(vector<vector<bitset<9>>*>::iterator it = columns->begin() ; it != columns->end(); ++it)
    {
        ++count;
        qTextEdit->insertPlainText("\nFor column : " + QString::number(count) + "\n");
        for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
    }
   /*cout<<"\n row assignment count : \n";
    for(int i = 1; i<=sizeOfTheProblem; i++)
    {
    	cout<<"\nFor Row "<<i<<" total assignments are :"<<rowAssignmentCount[i];
    }
    cout<<"\n Column assignment count : \n";
	for(int i = 1; i<=sizeOfTheProblem; i++)
	{
		cout<<"\nFor Column "<<i<<" total assignments are :"<<colAssignmentCount[i];
	}
	sortIndex(rowAssignmentCount, rowSortedIndex);
	sortIndex(colAssignmentCount, colSortedIndex);

	cout<<"\n array rowSortedIndex: \n";
    for(int i = 1; i<=sizeOfTheProblem; i++)
    cout<<"\t"<<rowSortedIndex[i];
    cout<<"\n array colSortedIndex: \n";
    for(int i = 1; i<=sizeOfTheProblem; i++)
    cout<<"\t"<<colSortedIndex[i];*/
}


void Kakurasu :: ACAlgorithm(vector<vector<bitset<9>>*> *temprow , vector<vector<bitset<9>>*> *tempcol, int **tempMatrix)
{

    qTextEdit->insertPlainText("\n ---------------AC Algorithm Begins----------------------------\n");
	for(int i = 0; i<sizeOfTheProblem; i++)
	{

		bitset<9> temp(0), temp1(pow(2,9)-1);
		for(vector<bitset<9>>::iterator itIn = temprow->at(i)->begin() ; itIn != temprow->at(i)->end(); ++itIn)
		{
			temp|=(*itIn);
			temp1&=(*itIn);
		}
		for(int j=sizeOfTheProblem-1; j>=0; j--)
		{
			if(temp[j]==0)
			{
				if(tempMatrix[i+1][j+1] == -1)
				{
					struct elementId e1;
					e1.indices[0] = i+1;
					e1.indices[1] = j+1;
					pQueue.insert(e1);
					tempMatrix[i+1][j+1] = 0;
				}
			}
			if(temp1[j] == 1)
			{
				if(tempMatrix[i+1][j+1] == -1)
				{
					struct elementId e1;
					e1.indices[0] = i+1;
					e1.indices[1] = j+1;
					pQueue.insert(e1);
					tempMatrix[i+1][j+1] = 1;
				}
			}
		}
		if(temprow->at(i)->size() == 1)
		{
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==1 and tempMatrix[i+1][j+1]==-1)
				{
					struct elementId e;
					e.indices[0] = i+1;
					e.indices[1] = j+1;
					pQueue.insert(e);
					tempMatrix[i+1][j+1]=1;

				}
				if(temp[j]==0 and tempMatrix[i+1][j+1]==-1)
				{
					struct elementId e;
					e.indices[0] = i+1;
					e.indices[1] = j+1;
					pQueue.insert(e);
					tempMatrix[i+1][j+1]=0;

				}
			}
		}
	}
	for(int i = 0; i<sizeOfTheProblem; i++)
	{
		bitset<9> temp(0), temp1(pow(2,9)-1);
		for(vector<bitset<9>>::iterator itIn = tempcol->at(i)->begin() ; itIn != tempcol->at(i)->end(); ++itIn)
		{
			temp|=(*itIn);
			temp1&=(*itIn);
		}
		for(int j=sizeOfTheProblem-1; j>=0; j--)
		{
			if(temp[j]==0)
			{
				if(tempMatrix[j+1][i+1] == -1)
				{
					struct elementId e1;
					e1.indices[0] = j+1;
					e1.indices[1] = i+1;
					pQueue.insert(e1);
					tempMatrix[j+1][i+1] = 0;
				}
			}
			if(temp1[j]==1)
			{
				if(tempMatrix[j+1][i+1] == -1)
				{
					struct elementId e1;
					e1.indices[0] = j+1;
					e1.indices[1] = i+1;
					pQueue.insert(e1);
					tempMatrix[j+1][i+1] = 1;
				}
			}
		}
		if(tempcol->at(i)->size() == 1)
		{
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==1 and tempMatrix[j+1][i+1]==-1)
				{
					struct elementId e;
					e.indices[0] = j+1;
					e.indices[1] = i+1;
					pQueue.insert(e);
					tempMatrix[j+1][i+1]=1;
				}
				if(temp[j]==0 and tempMatrix[j+1][i+1]==-1)
				{
					struct elementId e;
					e.indices[0] = j+1;
					e.indices[1] = i+1;
					pQueue.insert(e);
					tempMatrix[j+1][i+1]=0;
				}
			}
		}
	}

/********
 * pop each element from Queue and check for the respective temprow and tempcol, if any non-essentials or essentials assignments available
 */

    qTextEdit->insertPlainText("\n-------------------- Initial Queue Elements :---------------------------\n");
	for (std::set<elementId>::iterator itpQueue=pQueue.begin(); itpQueue!=pQueue.end(); ++itpQueue)
            qTextEdit->insertPlainText("\n\nqueue elements  : " + QString::number((*itpQueue).indices[0]) +
                    " and , " + QString::number((*itpQueue).indices[1]) + "\n");

	//for (std::set<elementId>::iterator itSet = pQueue.begin(); itSet != pQueue.end(); itSet++)
	while(!pQueue.empty())
	{
		std::set<elementId>::iterator itSet = pQueue.begin();
		struct elementId e1;
		e1.indices[0] = (*itSet).indices[0];
		e1.indices[1] = (*itSet).indices[1];

		pQueue.erase(itSet);
        qTextEdit->insertPlainText("\n popped element from queue is : " + QString::number(e1.indices[0]) + " , " + QString::number(e1.indices[1]));



//ac for temprow
		if(tempMatrix[e1.indices[0]][e1.indices[1]]==0)
		{
			for(vector<bitset<9>>::iterator itIn = temprow->at(e1.indices[0]-1)->begin() ; itIn != temprow->at(e1.indices[0]-1)->end(); ++itIn)
			{
				if((*itIn)[e1.indices[1]-1]==1)
				{
                    qTextEdit->insertPlainText("\n removed assignment is " + QString::fromStdString((*itIn).to_string()));
					itIn=temprow->at(e1.indices[0]-1)->erase(itIn);
					itIn--;
					rowAssignmentCount[e1.indices[0]]--;
				}

			}
			bitset<9> temp(0), temp1(pow(2,9)-1);
			for(vector<bitset<9>>::iterator itIn = temprow->at(e1.indices[0]-1)->begin() ; itIn != temprow->at(e1.indices[0]-1)->end(); ++itIn)
			{
				temp|=(*itIn);
				temp1&=(*itIn);
			}
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==0)
				{
					struct elementId e2;
					e2.indices[0] = e1.indices[0];
					e2.indices[1] = j+1;
					if(tempMatrix[e1.indices[0]][j+1]==-1)
					{
						pQueue.insert(e2);
						tempMatrix[e1.indices[0]][j+1] = 0;
                        qTextEdit->insertPlainText("\n Index number : " + QString::number(e1.indices[0]) + ", " + QString::number(j+1) + " = 0\n");
						printCheckMatrix(tempMatrix);
					}
				}
				if(temp1[j]==1)
				{
					struct elementId e2;
					e2.indices[0] = e1.indices[0];
					e2.indices[1] = j+1;
					if(tempMatrix[e1.indices[0]][j+1]==-1)
					{
						pQueue.insert(e2);
						tempMatrix[e1.indices[0]][j+1] = 1;
					}
				}
			}
			if(temprow->at(e1.indices[0]-1)->size() == 1)
			{
				for(int j=sizeOfTheProblem-1; j>=0; j--)
				{
					if(temp[j]==1 and tempMatrix[e1.indices[0]][j+1]==-1)
					{
						struct elementId e;
						e.indices[0] = e1.indices[0];
						e.indices[1] = j+1;
						pQueue.insert(e);
						tempMatrix[e1.indices[0]][j+1]=1;
					}
					if(temp[j]==0 and tempMatrix[e1.indices[0]][j+1]==-1)
					{
						struct elementId e;
						e.indices[0] = e1.indices[0];
						e.indices[1] = j+1;
						pQueue.insert(e);
						tempMatrix[e1.indices[0]][j+1]=0;
					}

				}
			}


//// ac for tempcol

			for(vector<bitset<9>>::iterator itIn = tempcol->at(e1.indices[1]-1)->begin() ; itIn != tempcol->at(e1.indices[1]-1)->end(); ++itIn)
			{
				if((*itIn)[e1.indices[0]-1]==1)
				{
                    qTextEdit->insertPlainText("\n removed assignment is " + QString::fromStdString((*itIn).to_string()));
					itIn = tempcol->at(e1.indices[1]-1)->erase(itIn);
					itIn--;
					colAssignmentCount[e1.indices[1]]--;
				}
			}
			temp = 0; temp1 = (pow(2,9)-1);
            qTextEdit->insertPlainText("\n \n temp1 = " + QString::fromStdString(temp1.to_string()) + "\n");
			for(vector<bitset<9>>::iterator itIn = tempcol->at(e1.indices[1]-1)->begin() ; itIn != tempcol->at(e1.indices[1]-1)->end(); ++itIn)
			{
				temp|=(*itIn);
				temp1&=(*itIn);
			}
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==0)
				{
					struct elementId e3;
					e3.indices[0] = j+1;
					e3.indices[1] = e1.indices[1];
					if(tempMatrix[j+1][e1.indices[1]]==-1)
					{
						pQueue.insert(e3);
						tempMatrix[j+1][e1.indices[1]] = 0;
					}
				}
				if(temp1[j]==1)
				{
					struct elementId e3;
					e3.indices[0] = j+1;
					e3.indices[1] = e1.indices[1];
					if(tempMatrix[j+1][e1.indices[1]]==-1)
					{
						pQueue.insert(e3);
						tempMatrix[j+1][e1.indices[1]] = 1;
					}
				}
			}
			if(tempcol->at(e1.indices[1]-1)->size() == 1)
			{
				for(int j=sizeOfTheProblem-1; j>=0; j--)
				{
					if(temp[j]==1 and tempMatrix[j+1][e1.indices[1]]==-1)
					{
						struct elementId e;
						e.indices[0] = j+1;
						e.indices[1] = e1.indices[1];
						pQueue.insert(e);
						tempMatrix[j+1][e1.indices[1]]=1;
					}
					if(temp[j]==0 and tempMatrix[j+1][e1.indices[1]]==-1)
					{
						struct elementId e;
						e.indices[0] = j+1;
						e.indices[1] = e1.indices[1];
						pQueue.insert(e);
						tempMatrix[j+1][e1.indices[1]]=0;
					}
				}
			}

		}
		else if(tempMatrix[e1.indices[0]][e1.indices[1]]==1)
		{
			for(vector<bitset<9>>::iterator itIn = temprow->at(e1.indices[0]-1)->begin() ; itIn != temprow->at(e1.indices[0]-1)->end(); ++itIn)
			{
				if((*itIn)[e1.indices[1]-1]==0)
				{
                    qTextEdit->insertPlainText("\n removed assignment is " + QString::fromStdString((*itIn).to_string()));
					itIn=temprow->at(e1.indices[0]-1)->erase(itIn);
					itIn--;
					rowAssignmentCount[e1.indices[0]]--;
				}

			}
			bitset<9> temp(0), temp1(pow(2,9)-1);
			for(vector<bitset<9>>::iterator itIn = temprow->at(e1.indices[0]-1)->begin() ; itIn != temprow->at(e1.indices[0]-1)->end(); ++itIn)
			{
				temp|=(*itIn);
				temp1&=(*itIn);
			}
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==0)
				{
					struct elementId e2;
					e2.indices[0] = e1.indices[0];
					e2.indices[1] = j+1;
					if(tempMatrix[e1.indices[0]][j+1]==-1)
					{
						pQueue.insert(e2);
						tempMatrix[e1.indices[0]][j+1] = 0;
					}
				}
				if(temp1[j]==1)
				{
					struct elementId e2;
					e2.indices[0] = e1.indices[0];
					e2.indices[1] = j+1;
					if(tempMatrix[e1.indices[0]][j+1]==-1)
					{
						pQueue.insert(e2);
						tempMatrix[e1.indices[0]][j+1] = 1;
					}
				}
			}
			if(temprow->at(e1.indices[0]-1)->size() == 1)
			{
				for(int j=sizeOfTheProblem-1; j>=0; j--)
				{
					if(temp[j]==1 and tempMatrix[e1.indices[0]][j+1]==-1)
					{
						struct elementId e;
						e.indices[0] = e1.indices[0];
						e.indices[1] = j+1;
						pQueue.insert(e);
						tempMatrix[e1.indices[0]][j+1]=1;
					}
					if(temp[j]==0 and tempMatrix[e1.indices[0]][j+1]==-1)
					{
						struct elementId e;
						e.indices[0] = e1.indices[0];
						e.indices[1] = j+1;
						pQueue.insert(e);
						tempMatrix[e1.indices[0]][j+1]=0;
					}

				}
			}

			/*cout<<"\n Remaining assignment of Row "<<e1.indices[0];
			for(vector<bitset<9>>::iterator itIn = temprow->at(e1.indices[0]-1)->begin() ; itIn != temprow->at(e1.indices[0]-1)->end(); ++itIn)
			{
				cout<<"\t"<<(*itIn);
			}

			cout<<"\n check Matrix : \n";
			for(int i =1; i<=sizeOfTheProblem; i++)
			{
				cout<<"\n";
				for(int j = 1; j<=sizeOfTheProblem; j++)
				{
					cout<<"\t"<<tempMatrix[i][j];
				}
			}
			cout<<"\n\n";*/


			//// ac for tempcol

			for(vector<bitset<9>>::iterator itIn = tempcol->at(e1.indices[1]-1)->begin() ; itIn != tempcol->at(e1.indices[1]-1)->end(); ++itIn)
			{
				if((*itIn)[e1.indices[0]-1]==0)
				{
                    qTextEdit->insertPlainText("\n removed assignment is " + QString::fromStdString((*itIn).to_string()));
					itIn = tempcol->at(e1.indices[1]-1)->erase(itIn);
					itIn--;
					colAssignmentCount[e1.indices[1]]--;
				}
			}
			temp = 0; temp1 = pow(2,9)-1;
			for(vector<bitset<9>>::iterator itIn = tempcol->at(e1.indices[1]-1)->begin() ; itIn != tempcol->at(e1.indices[1]-1)->end(); ++itIn)
			{
				temp|=(*itIn);
				temp1&=(*itIn);
			}
			for(int j=sizeOfTheProblem-1; j>=0; j--)
			{
				if(temp[j]==0)
				{
					struct elementId e3;
					e3.indices[0] = j+1;
					e3.indices[1] = e1.indices[1];
					if(tempMatrix[j+1][e1.indices[1]]==-1)
					{
						pQueue.insert(e3);
						tempMatrix[j+1][e1.indices[1]] = 0;
					}

				}
				if(temp1[j]==1)
				{
					struct elementId e3;
					e3.indices[0] = j+1;
					e3.indices[1] = e1.indices[1];
					if(tempMatrix[j+1][e1.indices[1]]==-1)
					{
						pQueue.insert(e3);
						tempMatrix[j+1][e1.indices[1]] = 1;
					}

				}
			}
			if(tempcol->at(e1.indices[1]-1)->size() == 1)
			{
				for(int j=sizeOfTheProblem-1; j>=0; j--)
				{
					if(temp[j]==1 and tempMatrix[j+1][e1.indices[1]]==-1)
					{
						struct elementId e;
						e.indices[0] = j+1;
						e.indices[1] = e1.indices[1];
						pQueue.insert(e);
						tempMatrix[j+1][e1.indices[1]]=1;
					}
					if(temp[j]==0 and tempMatrix[j+1][e1.indices[1]]==-1)
					{
						struct elementId e;
						e.indices[0] = j+1;
						e.indices[1] = e1.indices[1];
						pQueue.insert(e);
						tempMatrix[j+1][e1.indices[1]]=0;
					}
				}
			}

			/*cout<<"\n check Matrix : \n";
			for(int i =1; i<=sizeOfTheProblem; i++)
			{
				cout<<"\n";
				for(int j = 1; j<=sizeOfTheProblem; j++)
				{
					cout<<"\t"<<tempMatrix[i][j];
				}
			}
			cout<<"\n\n";*/
		}
	}


    qTextEdit->insertPlainText("\n\n\n\n\n---------------------After AC Algorithm---------------------------------------------\n\n\n");

	/*for (std::set<elementId>::iterator itpQueue=pQueue.begin(); itpQueue!=pQueue.end(); ++itpQueue)
		cout<<"\n\n queue elements  : "<<(*itpQueue).indices[0]<<" and , "<<(*itpQueue).indices[1]<<"\n";
*/
    qTextEdit->insertPlainText("\n check Matrix : \n");
	for(int i =1; i<=sizeOfTheProblem; i++)
	{
        qTextEdit->insertPlainText("\n");
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
            qTextEdit->insertPlainText("\t" + QString::number(tempMatrix[i][j]));
		}
	}
	int count = 0;
	for(vector<vector<bitset<9>>*>::iterator it = temprow->begin() ; it != temprow->end(); ++it)
	{
        ++count;
        qTextEdit->insertPlainText("\nFor row : " + QString::number(count) + "\n");
		for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
	}

    qTextEdit->insertPlainText("\nFor all tempcol\n");
	count = 0;
	for(vector<vector<bitset<9>>*>::iterator it = tempcol->begin() ; it != tempcol->end(); ++it)
	{
        ++count;
        qTextEdit->insertPlainText("\nFor column : " + QString::number(count) + "\n");
		for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
	}
}

void Kakurasu :: printCheckMatrix(int **tempCM)
{
    qTextEdit->insertPlainText("\n check Matrix : \n");
	for(int i =1; i<=sizeOfTheProblem; i++)
	{
        qTextEdit->insertPlainText("\n");
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
            qTextEdit->insertPlainText("\t" + QString::number(tempCM[i][j]));
		}
	}
}

void Kakurasu :: printfMatrix(int **tempFM)
{
    qTextEdit->insertPlainText("\n F Matrix is  : \n");
		for(int i =1; i<=sizeOfTheProblem; i++)
		{
            qTextEdit->insertPlainText("\n");
			for(int j = 1; j<=sizeOfTheProblem; j++)
			{
                qTextEdit->insertPlainText("\t" + QString::number(tempFM[i][j]));
			}
		}
}



int Kakurasu :: unidentifiedVariables(int **tempMat, int **ftempMatrix)
{
	int temp = 0;
	for(int i =1; i<=sizeOfTheProblem; i++)
	{
		for(int j = 1; j<= sizeOfTheProblem; j++)
		{
			if(tempMat[i][j] == -1)
			{
				ftempMatrix[i][j] = tempMat[i][j];
				temp+=1;
			}
			else
			{
				ftempMatrix[i][j] = 1;
			}
		}
	}
	return temp;
}

void Kakurasu :: stm(list<fValue> &fList, int **tempMat, int **ftempMatrix)
{
	bool flag = false;
	for(int i = 1; i<= sizeOfTheProblem; i++)
	{
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
			if(tempMat[i][j] == -1)
			{
				struct fValue v1;
				v1.rindex = i;
				v1.cindex = j;
				v1.scoreValue = 2*FValueCalculate(i,j,fList);
				fList.push_back(v1);
				ftempMatrix[i][j] = 1;
				flag = true;
				break;
			}
		}
		if(flag == true)
		{
			break;
		}
	}
	int next_row_index, next_col_index, nextfValue;
	float minScore,score;
	for(int i = 1; i<=UnidentifiedVariables; i++)
	{
		for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
		{
			int row_number = (*it).rindex;
			int col_number = (*it).cindex;
			for(int j = 1; j<=sizeOfTheProblem; j++)
			{
				if(ftempMatrix[row_number][j] == -1)
				{
					struct elementId e;
					e.indices[0] = row_number;
					e.indices[1] = j;
					elementsToCheck.push_back(e);
				}
			}
			for(int j = 1; j<=sizeOfTheProblem; j++)
			{
				if(ftempMatrix[j][col_number] == -1)
				{
					struct elementId e;
					e.indices[0] = j;
					e.indices[1] = col_number;
					elementsToCheck.push_back(e);
				}
			}
		}
		minScore = 9999.0;
		for(list<elementId>:: iterator itIn = elementsToCheck.begin(); itIn != elementsToCheck.end(); itIn++)
		{
			int row_index = (*itIn).indices[0];
			int col_index = (*itIn).indices[1];
			score = 2.0 * FValueCalculate(row_index, col_index, fList);

			if(score < minScore)
			{
				minScore = score;
				next_row_index = row_index;
				next_col_index = col_index;
				nextfValue = minScore;
			}
		}
		if(!elementsToCheck.empty())
		{
			struct fValue v1;
			v1.cindex = next_col_index;
			v1.rindex = next_row_index;
			v1.scoreValue = nextfValue;
			fList.push_back(v1);
			ftempMatrix[next_row_index][next_col_index] = 1;
		}
		while(!elementsToCheck.empty())
		{
			elementsToCheck.erase(elementsToCheck.begin());
		}
	}

    qTextEdit->insertPlainText("\n ftempMatrix  is : \n");
	for(int i = 1; i<=sizeOfTheProblem; i++)
	{
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
            qTextEdit->insertPlainText("\t" + QString::number(ftempMatrix[i][j]));
		}
        qTextEdit->insertPlainText("\n");
	}

    qTextEdit->insertPlainText("\nF values are : \n");
	for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
	{
        qTextEdit->insertPlainText("(" + QString::number((*it).rindex) + " , " + QString::number((*it).cindex) + " , " + QString::number((*it).scoreValue) + ")\n");
	}

}

float Kakurasu :: FValueCalculate(int row, int col, list<fValue> fList)
{
	if(fList.empty())
	{
		return 1;
	}
	else
	{
/////F Value for row constraints
		for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
		{
			if((*it).rindex == row)
			{
				col_position.push_back((*it).cindex);
			}
		}
		const int t = col_position.size();
		for(vector<bitset<9>>::iterator itIn = rows->at(row-1)->begin() ; itIn != rows->at(row-1)->end(); ++itIn)
		{
			bitset<9> temp(0);
			list<int>::iterator it1;
			for(int i = 0; i<t; i++)
			{
				it1 = next(col_position.begin(), i);
				temp[i] = (*itIn)[(*it1)-1];
			}
			struct bits combination1;
			combination1.e = temp;
			colVariableCombinations.insert(combination1);
		}
		int sizeOfCombinationWithLessVariable = colVariableCombinations.size();
		col_position.push_back(col);
		//set<bitset<9>> colVariableCombinations1;
		const int t0 = col_position.size();
		for(vector<bitset<9>>::iterator itIn = rows->at(row-1)->begin() ; itIn != rows->at(row-1)->end(); ++itIn)
		{
			bitset<9> temp(0);
			list<int>::iterator it1;
			for(int i = 0; i<t0; i++)
			{
				it1 = next(col_position.begin(), i);
				temp[i] = (*itIn)[(*it1)-1];
			}
			struct bits combination2;
			combination2.e = temp;
			colVariableCombinations1.insert(combination2);
		}
		int sizeOfCombinationWithNextVariable = colVariableCombinations1.size();
		float f1 = (float)sizeOfCombinationWithNextVariable/sizeOfCombinationWithLessVariable;

/////F Value for column constraints
		for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
		{
			if((*it).cindex == col)
			{
				row_position.push_back((*it).rindex);
			}
		}
		//set<bitset<9>> rowVariableCombinations;
		const int t1 = row_position.size();
		for(vector<bitset<9>>::iterator itIn = columns->at(col-1)->begin() ; itIn != columns->at(col-1)->end(); ++itIn)
		{
			bitset<9> temp(0);
			list<int>::iterator it1;
			for(int i = 0; i<t1; i++)
			{
				it1 = next(row_position.begin(), i);
				temp[i] = (*itIn)[(*it1)-1];
			}
			struct bits combination3;
			combination3.e = temp;
			rowVariableCombinations.insert(combination3);
		}
		sizeOfCombinationWithLessVariable = rowVariableCombinations.size();
		row_position.push_back(row);
		//set<bitset<9>> rowVariableCombinations1;
		const int t2 = row_position.size();
		for(vector<bitset<9>>::iterator itIn = columns->at(col-1)->begin() ; itIn != columns->at(col-1)->end(); ++itIn)
		{
			bitset<9> temp(0);
			list<int>::iterator it1;
			for(int i = 0; i<t2; i++)
			{
				it1 = next(row_position.begin(), i);
				temp[i] = (*itIn)[(*it1)-1];
			}
			struct bits combination4;
			combination4.e = temp;
			rowVariableCombinations1.insert(combination4);
		}
		sizeOfCombinationWithNextVariable = rowVariableCombinations1.size();
		float f2 = (float)sizeOfCombinationWithNextVariable/sizeOfCombinationWithLessVariable;
		colVariableCombinations.clear();
		colVariableCombinations1.clear();
		rowVariableCombinations.clear();
		rowVariableCombinations1.clear();
		return (0.5*f1*f2);
	}
}


void Kakurasu :: searchalgorithm(vector<vector<bitset<9>>*> *temprow , vector<vector<bitset<9>>*> *tempcol, int **tempCM, int **tempFM, list<fValue> tempList)
{
    qTextEdit->insertPlainText("\n\n-------------------Search Algorithm Begins---------------------------\n");


	int **tempCheckMatrix, **ftempMatrix;
	tempCheckMatrix = new int* [sizeOfTheProblem+1];
	ftempMatrix = new int* [sizeOfTheProblem+1];

	for(int i = 0; i<=sizeOfTheProblem; i++)
	{
		tempCheckMatrix[i] = new int[sizeOfTheProblem+1];
		ftempMatrix[i] = new int[sizeOfTheProblem+1];
	}

	for(int i = 1; i<=sizeOfTheProblem; i++)
	{
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
			tempCheckMatrix[i][j] = tempCM[i][j];
			ftempMatrix[i][j] = tempFM[i][j];
		}
	}

	int unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);

    qTextEdit->insertPlainText("\n\n Inside Search Function : \n\n");
	printCheckMatrix(tempCM);
	printfMatrix(tempFM);


	fflush(stdout);

	unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);


	while(unIdentify > 0)
	{
		vector<vector<bitset<9>>*> *tempRowAssignments, *tempRowAssignments1;
		vector<vector<bitset<9>>*> *tempColumnAssignments, *tempColumnAssignments1;


		tempRowAssignments = new vector<vector<bitset<9>>*>;
		tempColumnAssignments = new vector<vector<bitset<9>>*>;
		tempRowAssignments1 = new vector<vector<bitset<9>>*>;
		tempColumnAssignments1 = new vector<vector<bitset<9>>*>;
		for(int i = 0; i<=sizeOfTheProblem - 1; i++)
		{
			tempRowAssignments->push_back((new vector<bitset<9>>));
			tempColumnAssignments->push_back((new vector<bitset<9>>));
			tempRowAssignments1->push_back((new vector<bitset<9>>));
			tempColumnAssignments1->push_back((new vector<bitset<9>>));
		}

		int count = 0;
		for(vector<vector<bitset<9>>*>::iterator it = temprow->begin() ; it != temprow->end(); ++it)
		{
			for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
			{
				tempRowAssignments->at(count)->push_back(*itIn);
				tempRowAssignments1->at(count)->push_back(*itIn);
			}
			count++;
		}

		count = 0;
		for(vector<vector<bitset<9>>*>::iterator it = tempcol->begin() ; it != tempcol->end(); ++it)
		{
			for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
			{
				tempColumnAssignments->at(count)->push_back(*itIn);
				tempColumnAssignments1->at(count)->push_back(*itIn);
			}
			count++;
		}

		list<fValue> fList;
		fList = tempList;

        qTextEdit->insertPlainText("\n in Search function : fList is :\n");
		for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
		{
            qTextEdit->insertPlainText("(" + QString::number((*it).rindex) + " , " + QString::number((*it).cindex) + " , " + QString::number((*it).scoreValue) + ")\n");
		}

		fflush(stdout);

		struct fValue it1 = fList.front();
		int row_number = (it1).rindex;
		int col_number = (it1).cindex;
        qTextEdit->insertPlainText("\n Row Number : " + QString::number(row_number) + "\t Column Number :" + QString::number(col_number) + "\n");
		fList.pop_front();
		fList.clear();

        qTextEdit->insertPlainText("\n in Search function fList after clearing :\n");
		for(list<fValue>::iterator it = fList.begin(); it != fList.end(); it++)
		{
			if(fList.size() == 0)
			{
                qTextEdit->insertPlainText("\n\n fList is empty because of clear fucntion  \n\n");
				break;
			}
			else
			{
                qTextEdit->insertPlainText("(" + QString::number((*it).rindex) + " , " + QString::number((*it).cindex) + " , " + QString::number((*it).scoreValue) + ")\n");
			}
		}

		if(tempCheckMatrix[row_number][col_number] == -1)
		{
            qTextEdit->insertPlainText("\n\n\n\n\n--------------------------trying first undefined = 1----------------------------------------\n\n\n");
			tempCheckMatrix[row_number][col_number] = 1;
			ftempMatrix[row_number][col_number] = 1;
            qTextEdit->insertPlainText("\n\n Check Matrix and F Matrix after assigning first undefined variable = 1 \n");
			printCheckMatrix(tempCheckMatrix);
			printfMatrix(ftempMatrix);
			for(vector<bitset<9>>::iterator itIn = tempRowAssignments->at(row_number-1)->begin() ; itIn != tempRowAssignments->at(row_number-1)->end(); ++itIn)
			{
				if((*itIn)[col_number-1]==0)
				{
                    qTextEdit->insertPlainText("\n removed assignment from row  " + QString::number(row_number) + " is : " + QString::fromStdString((*itIn).to_string()));
					itIn=tempRowAssignments->at(row_number-1)->erase(itIn);
					itIn--;
				}
			}
			for(vector<bitset<9>>::iterator itIn = tempColumnAssignments->at(col_number-1)->begin() ; itIn != tempColumnAssignments->at(col_number-1)->end(); ++itIn)
			{
				if((*itIn)[row_number-1]==0)
				{
                    qTextEdit->insertPlainText("\n removed assignment from Column " + QString::number(col_number) + " is : " + QString::fromStdString((*itIn).to_string()));
					itIn=tempColumnAssignments->at(col_number-1)->erase(itIn);
					itIn--;
				}
			}
			ACAlgorithm(tempRowAssignments, tempColumnAssignments, tempCheckMatrix);
			int flag1 = 0, flag2 = 0;
			for(int i = 0; i<sizeOfTheProblem; i++)
			{
				if(tempRowAssignments->at(i)->size()==0)
				{
					flag1 = 1;
					break;
				}
			}
			for(int i = 0; i<sizeOfTheProblem; i++)
			{
				if(tempColumnAssignments->at(i)->size()==0)
				{
					flag2 = 1;
					break;
				}
			}
			if(flag1 == 1 or flag2 == 1)
			{
                qTextEdit->insertPlainText("\n \n\nSome of the constraints are not satisfied, so back tracking to the last assignments \n");
                qTextEdit->insertPlainText("\n\n\n\n\n--------------------------trying first undefined = 0----------------------------------------\n\n\n");
				int count = 0;
				for(vector<vector<bitset<9>>*>::iterator it = tempRowAssignments1->begin() ; it != tempRowAssignments1->end(); ++it)
				{
                    ++count;
                    qTextEdit->insertPlainText("\nFor row : " + QString::number(count) + "\n");
					for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
                        qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
				}

                qTextEdit->insertPlainText("\nFor all columns\n");
				count = 0;
				for(vector<vector<bitset<9>>*>::iterator it = tempColumnAssignments1->begin() ; it != tempColumnAssignments1->end(); ++it)
				{
                    ++count;
                    qTextEdit->insertPlainText("\nFor row : " + QString::number(count) + "\n");
					for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
                        qTextEdit->insertPlainText(QString::fromStdString((*itIn).to_string()) + "\t");
				}





				for(int i = 1; i<=sizeOfTheProblem; i++)
				{
					for(int j = 1; j<=sizeOfTheProblem; j++)
					{
						tempCheckMatrix[i][j] = tempCM[i][j];
						ftempMatrix[i][j] = tempFM[i][j];
					}
				}
				unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);
				tempCheckMatrix[row_number][col_number] = 0;
				ftempMatrix[row_number][col_number] = 1;
				for(vector<bitset<9>>::iterator itIn = tempRowAssignments1->at(row_number-1)->begin() ; itIn != tempRowAssignments1->at(row_number-1)->end(); ++itIn)
				{
					if((*itIn)[col_number-1]==1)
					{
                        qTextEdit->insertPlainText("\n removed assignment from row  " + QString::number(row_number) + " is : " + QString::fromStdString((*itIn).to_string()));
						itIn=tempRowAssignments1->at(row_number-1)->erase(itIn);
						itIn--;
					}
				}
				for(vector<bitset<9>>::iterator itIn = tempColumnAssignments1->at(col_number-1)->begin() ; itIn != tempColumnAssignments1->at(col_number-1)->end(); ++itIn)
				{
					if((*itIn)[row_number-1]==1)
					{
                        qTextEdit->insertPlainText("\n removed assignment from Column " + QString::number(col_number) + " is : " + QString::fromStdString((*itIn).to_string()));
						itIn=tempColumnAssignments1->at(col_number-1)->erase(itIn);
						itIn--;
					}
				}
				ACAlgorithm(tempRowAssignments1, tempColumnAssignments1, tempCheckMatrix);
				int flag3 = 0, flag4 = 0;
				for(int i = 0; i<sizeOfTheProblem; i++)
				{
					if(tempRowAssignments1->at(i)->size()==0)
					{
						flag3 = 1;
						break;
					}
				}
				for(int i = 0; i<sizeOfTheProblem; i++)
				{
					if(tempColumnAssignments1->at(i)->size()==0)
					{
						flag4 = 1;
						break;
					}
				}
				if(flag3 == 1 or flag4 == 1)
				{
                    qTextEdit->insertPlainText("\n answer is not consistent please backtrack");
					break;
				}
				else if(unidentifiedVariables(tempCheckMatrix, ftempMatrix) > 0)
				{
					rows = tempRowAssignments1;
					columns = tempColumnAssignments1;
					unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);
					stm(fList,tempCheckMatrix, ftempMatrix);

                    qTextEdit->insertPlainText("\n flag3,4 is not zero, unIdentify = " + QString::number(unIdentify) + "\n");
					if(unIdentify > 0)
					{
						searchalgorithm(tempRowAssignments1, tempColumnAssignments1, tempCheckMatrix, ftempMatrix, fList);
					}
				}

			}
			else if(unidentifiedVariables(tempCheckMatrix, ftempMatrix) > 0)
			{
				rows = tempRowAssignments;
				columns = tempColumnAssignments;
				unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);
				stm(fList,tempCheckMatrix, ftempMatrix);

                qTextEdit->insertPlainText("\n flag1,2 is not zero, unIdentify = " + QString::number(unIdentify) + "\n");
				if(unIdentify > 0)
				{
					searchalgorithm(tempRowAssignments, tempColumnAssignments, tempCheckMatrix, ftempMatrix, fList);
				}
			}
		}
        qTextEdit->insertPlainText("\n");
		if(solutionFound)
		{
			fflush(stdout);
			return;
		}
		else
		{
            qTextEdit->insertPlainText("\n F and Check Matrices at the last of while loop : ");
			unIdentify = unidentifiedVariables(tempCheckMatrix, ftempMatrix);
			printCheckMatrix(tempCheckMatrix);
			printfMatrix(ftempMatrix);
            qTextEdit->insertPlainText("\n unIdentify at the last in while loop = " + QString::number(unIdentify) + "\n");
		}
	}




	solutionFound = true;

    qTextEdit->insertPlainText("\n\n Solution is : \n");

    qTextEdit->insertPlainText("\n check Matrix : \n");
		for(int i =1; i<=sizeOfTheProblem; i++)
		{
            qTextEdit->insertPlainText("\n");
			for(int j = 1; j<=sizeOfTheProblem; j++)
			{
                qTextEdit->insertPlainText("\t" + QString::number(tempCheckMatrix[i][j]));
			}
		}

    qTextEdit->insertPlainText("\n");

	for(int i =1; i<=sizeOfTheProblem; i++)
	{
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
			if(tempCheckMatrix[i][j] == 1)
			{
				solMatrix[i][j] = true;
			}
			else
			{
				solMatrix[i][j] = false;
			}
		}
	}
	for(int i =1; i<=sizeOfTheProblem; i++)
	{
		for(int j = 1; j<=sizeOfTheProblem; j++)
		{
			checkMatrix[i][j] = tempCheckMatrix[i][j];
		}
	}

}



void Kakurasu :: sortIndex(int *a, int *b)
{
	int temp = 0;
	  for(int i = 1; i<=sizeOfTheProblem; i++)
	  {
	      for(int j = 1; j<sizeOfTheProblem; j++)
	      {
	          if(a[b[j]]>a[b[j+1]])
	          {
	              temp = b[j];
	              b[j] = b[j+1];
	              b[j+1] = temp;
	          }
	      }
	  }
}
