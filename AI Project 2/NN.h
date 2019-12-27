#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include <vector>
#include <cmath>

using namespace std;

class NeuralNet {
private:

class Data {
public:
vector<double> inputVal;
vector<double> outputVal;
};

class DataStats {
public:
int a;
int b;
int c;
int d;
double acc;
double prec;
double rec;
double F1;
};

class Node;

class Edge {
public:
double weight;
Node * previous;
Node * next;
int pr[2];
int nx[2];
};

class Node {
public:
bool bias;
vector<Edge *> inputEdge;
vector<Edge *> outputEdge;

double activationFn;
double Del;
double inputTotal;
};

public:
vector<vector<Node *> > nodez;
vector<Edge *> List;
void trainNN(ifstream &NNFile, ifstream &trainFile, ofstream &outFile, int epoch, double LR);
void testNN(ifstream &NNFile, ifstream &testFile, ofstream &outFile);
void readNN(ifstream &inputFile);
};
