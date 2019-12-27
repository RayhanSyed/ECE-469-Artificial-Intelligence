#include "NN.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void NeuralNet::trainNN(ifstream &NNFile, ifstream &trainFile, ofstream &outFile, int epoch, double LR) {
        readNN(NNFile);     //Read neaural network file
        double nValue;      //Declare variables
        long long ExampleNo;
        long long InputNo;
        long long OutputNo;
        vector<Data> trainSet;
        Node *node;
        Edge *edge;
        double total;
        int count = 0;
        trainFile >> ExampleNo >> InputNo >> OutputNo; //Obtain NN characteristics
        for(int i = 0; i < ExampleNo; i++) {
                NeuralNet::Data dataTemp;
                for(int j = 0; j < (InputNo + OutputNo); j++) {
                        trainFile >> nValue;
                        if(j<InputNo)
                                dataTemp.inputVal.push_back(nValue);  // Get innput nodes
                        else
                                dataTemp.outputVal.push_back(nValue);  // Get output node
                }
                trainSet.push_back(dataTemp);
        }
        while (count < epoch) {
                for(int i = 0; i < trainSet.size(); i++) {
                        for(int j = 1; j < this->nodez[0].size(); j++) { //get inout value
                                this->nodez[0][j]->activationFn = trainSet[i].inputVal[j-1];
                        }
                        for(int j = 1; j < this->nodez.size(); j++) { // Hidden and output layers
                                for(int k = 0; k < this->nodez[j].size(); k++) {
                                        if(this->nodez[j][k]->bias) {
                                                this->nodez[j][k]->inputTotal = 0.0;
                                        }
                                        else {
                                                total = 0.0;
                                                for(int l = 0; l < this->nodez[j][k]->inputEdge.size(); l++) {
                                                        total = total + this->nodez[j][k]->inputEdge[l]->weight * this->nodez[j][k]->inputEdge[l]->previous->activationFn;
                                                }
                                                this->nodez[j][k]->inputTotal = total;
                                                this->nodez[j][k]->activationFn = (1.0 / (1.0 + exp(-1.0*total)));
                                        }
                                }
                        }
                        for(int j = 0; j < this->nodez[this->nodez.size() - 1].size(); j++) { // get output value
                                node = this->nodez[this->nodez.size() - 1][j];
                                node->Del = (1.0 / (1.0 + exp(-1.0*(node->inputTotal)))) * (1.0- (1.0 / (1.0 + exp(-1.0*(node->inputTotal)))))  * (trainSet[i].outputVal[j] - node->activationFn);
                        }
                        for(int j = this->nodez.size() - 2; j > 0; j--) {
                                for(int k = 0; k < this->nodez[j].size(); k++) {
                                        node = this->nodez[j][k];
                                        total = 0.0;
                                        for(int l = 0; l < node->outputEdge.size(); l++) {
                                                total = total + node->outputEdge[l]->weight * node->outputEdge[l]->next->Del;
                                        }
                                        node->Del = (1.0 / (1.0 + exp(-1.0*(node->inputTotal)))) * (1.0- (1.0 / (1.0 + exp(-1.0*(node->inputTotal)))))  * total;
                                }
                        }
                        for(int j = 0; j < this->List.size(); j++) {
                                edge = this->List[j];
                                edge->weight = edge->weight + (LR * edge->previous->activationFn * edge->next->Del);
                        }
                }
                count++;
        }
        // Wrtie to trained NN file
        for(int i = 0; i < this->nodez.size(); i++) {
                if(i == this->nodez.size() - 1) {
                        outFile << this->nodez[i].size();
                }
                else{
                        outFile << this->nodez[i].size() - 1 << " ";
                }
        }

        outFile << endl;
        for(int i = 1; i < this->nodez.size(); i++) {
                for(int j = 0; j < this->nodez[i].size(); j++) {
                        if((this->nodez[i][j]->bias)==false) {
                                for(int k = 0; k < this->nodez[i][j]->inputEdge.size(); k++) {
                                        outFile << setprecision(3) << fixed << this->nodez[i][j]->inputEdge[k]->weight;
                                        if(k != this->nodez[i][j]->inputEdge.size()-1) {
                                                outFile << " ";
                                        }
                                }
                                outFile << endl;
                        }
                }

        }

}

void NeuralNet::testNN(ifstream &NNFile, ifstream &testFile, ofstream &outFile) {
        readNN(NNFile);     //Read neaural network file
        long long count;    //Declare variables
        long long InputNo;
        long long OutputNo;
        double nValue;
        double total;
        vector<Data> testSet;
        vector<DataStats> testingOut;
        Data dataTemp;
        DataStats dataPoint;
        dataPoint.a = 0;    //Initialize false positive, false negative, true positive, true negative values
        dataPoint.b = 0;
        dataPoint.c = 0;
        dataPoint.d = 0;
        testFile >> count >> InputNo >> OutputNo;
        for(int i = 0; i < count; i++) {
                for(int j = 0; j < (InputNo + OutputNo); j++) {
                        testFile >> nValue;
                        if(j<InputNo)
                                dataTemp.inputVal.push_back(nValue);  // Get input node
                        else
                                dataTemp.outputVal.push_back(nValue);  // get output node
                }
                testSet.push_back(dataTemp);
                dataTemp.inputVal.clear();
                dataTemp.outputVal.clear();
        }
        for(int i = 0; i < this->nodez[this->nodez.size() - 1].size(); i++) {
                testingOut.push_back(dataPoint);
        }
        for(int i = 0; i < testSet.size(); i++) {
                for(int j = 1; j < this->nodez[0].size(); j++) {
                        this->nodez[0][j]->activationFn = testSet[i].inputVal[j - 1];
                }
                for(int j = 1; j < this->nodez.size(); j++) {
                        for(int k = 0; k < this->nodez[j].size(); k++) {
                                if(this->nodez[j][k]->bias) {
                                        this->nodez[j][k]->inputTotal = 0.0;
                                }
                                else {
                                        total = 0.0;
                                        for(int l = 0; l < this->nodez[j][k]->inputEdge.size(); l++) {
                                                total = total + this->nodez[j][k]->inputEdge[l]->weight * this->nodez[j][k]->inputEdge[l]->previous->activationFn;
                                        }
                                        this->nodez[j][k]->inputTotal = total;
                                        this->nodez[j][k]->activationFn = (1.0 / (1.0 + exp(-1.0*total)));
                                }
                        }
                }
                for(int j = 0; j < testingOut.size(); j++) {
                        int m = round(this->nodez[this->nodez.size() - 1][j]->activationFn);
                        if (m) {
                                if(testSet[i].outputVal[j]) {
                                        testingOut[j].a = testingOut[j].a + 1;
                                }
                                if(!(testSet[i].outputVal[j])) {
                                        testingOut[j].b = testingOut[j].b + 1;
                                }
                        }
                        else if(!m) {
                                if(testSet[i].outputVal[j]) {
                                        testingOut[j].c = testingOut[j].c + 1;
                                }
                                if(!(testSet[i].outputVal[j])) {
                                        testingOut[j].d = testingOut[j].d + 1;
                                }
                        }
                }
        }
        outFile << setprecision(3) << fixed; //handle result of test set and write to file
        double a;
        double b;
        double c;
        double d;
        double acc; //accuracy
        double prec; //precision
        double rec; //recall
        double F1; //F1
        for(int i = 0; i < testingOut.size(); i++) {
                a = testingOut[i].a;
                b = testingOut[i].b;
                c = testingOut[i].c;
                d = testingOut[i].d;
                testingOut[i].acc = (a + d) / (a + b + c + d);
                testingOut[i].prec = a / (a + b);
                testingOut[i].rec = a / (a + c);
                testingOut[i].F1 = (2 * testingOut[i].prec * testingOut[i].rec) / (testingOut[i].prec + testingOut[i].rec);
                outFile << (int)a << " " << (int)b << " " << (int)c << " " << (int)d << " ";
                outFile << testingOut[i].acc << " " << testingOut[i].prec << " " << testingOut[i].rec << " " << testingOut[i].F1 << endl;
        }
        a = 0.0;
        b = 0.0;
        c = 0.0;
        d = 0.0;
        acc = 0.0;
        prec = 0.0;
        rec = 0.0;
        F1 = 0.0;
        for(int i = 0; i < testingOut.size(); i++) {
                a = a + testingOut[i].a;
                b = b + testingOut[i].b;
                c = c + testingOut[i].c;
                d = d + testingOut[i].d;
        }
        outFile << (a + d) / (a + b + c + d) << " " << a / (a + b) << " " << a / (a + c) << " " << ((2 * (a / (a + b)) * (a / (a + c))) / ((a / (a + b)) + (a / (a + c)))) << endl;
        for(int i = 0; i < testingOut.size(); i++) {
                acc =acc + testingOut[i].acc;
                prec = prec + testingOut[i].prec;
                rec =rec + testingOut[i].rec;
        }
        acc = acc / testingOut.size();
        prec = prec / testingOut.size();
        rec = rec / testingOut.size();
        outFile << acc << " " << prec << " " << rec << " " << ((2 * prec * rec) / (prec + rec)) << endl;
}
void NeuralNet::readNN(ifstream &inputFile) {
        vector<int> NumCount; //Declare variables
        vector<double> edgeWeight;
        vector<vector<double> > NNode;
        vector<vector<vector<double> > > layers;
        string buffer;
        string tok;
        getline(inputFile, buffer);
        istringstream Str(buffer);

        while(Str >> tok) {        // Read input
                NumCount.push_back(stoul(tok, nullptr, 0));
        }

        for(int i = 0; i < NumCount[0]+1; i++) {
                edgeWeight.push_back(1);
                NNode.push_back(edgeWeight);
                edgeWeight.clear();
        }

        layers.push_back(NNode);
        NNode.clear();

        for(int i = 1; i < NumCount.size(); i++) {
                if(i + 1< NumCount.size()) {
                        edgeWeight.push_back(1);
                        NNode.push_back(edgeWeight);
                        edgeWeight.clear();
                }

                for(int j = 0; j < NumCount[i]; j++) {
                        getline(inputFile, buffer);
                        Str = istringstream(buffer);
                        while(Str >> tok) {
                                double strNo = strtod(tok.c_str(), nullptr);
                                edgeWeight.push_back(strNo);
                        }
                        NNode.push_back(edgeWeight);
                        edgeWeight.clear();
                }
                layers.push_back(NNode);
                NNode.clear();
        }
        Node* N;          // Set up edgeweights
        Edge * E;
        vector<Node *> lNodes;

        for(int i = 0; i < layers[0].size(); i++) {
                N = new Node;
                N->activationFn = (i ? 0.0 : -1.0);
                N->bias = (i ? false :  true);
                N->inputEdge.push_back(nullptr);
                lNodes.push_back(N);
        }
        this->nodez.push_back(lNodes);
        lNodes.clear();
        for(int i = 1; i < layers.size() - 1; i++) {                  // Setup hidden layer
                for(int j = 0; j < layers[i].size(); j++) {
                        N = new Node;
                        N->activationFn = (j ? 0.0 : -1.0);
                        N->bias = (j ? false :  true);
                        if(j) {
                                for(int k = 0; k < layers[i][j].size(); k++) {
                                        E = new Edge;
                                        E->weight = layers[i][j][k];
                                        E->previous = this->nodez[i-1][k];
                                        E->next = N;
                                        E->pr[0] = i - 1;
                                        E->pr[1] = k;
                                        E->nx[0] = i;
                                        E->nx[1] = j;
                                        this->List.push_back(E);
                                        this->nodez[i-1][k]->outputEdge.push_back(E);
                                        N->inputEdge.push_back(E);
                                }
                        }
                        else {
                                N->inputEdge.push_back(nullptr);
                        }
                        lNodes.push_back(N);
                }
                this->nodez.push_back(lNodes);
                lNodes.clear();
        }
        for(int i = 0; i < layers[layers.size()-1].size(); i++) {     // Setup output layer
                N = new Node;
                N->activationFn = 0.0;
                N->bias = false;
                for(int j = 0; j < layers[layers.size()-1][i].size(); j++) {
                        E = new Edge;
                        E->weight = layers[layers.size() - 1][i][j];
                        E->previous = this->nodez[layers.size() - 2][j];
                        E->next = N;
                        E->pr[0] = layers.size() - 2;
                        E->pr[1] = j;
                        E->nx[0] = layers.size() - 1;
                        E->nx[1] = i;
                        this->List.push_back(E);
                        this->nodez[layers.size() - 2][j]->outputEdge.push_back(E);
                        N->inputEdge.push_back(E);
                }

                lNodes.push_back(N);
        }
        this->nodez.push_back(lNodes);
        lNodes.clear();
}
