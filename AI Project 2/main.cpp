#include <iostream>
#include <fstream>
#include <string>
#include "NN.h"

using namespace std;

int train() {
        string inFile;    //Declare varible for parsing files, epoch data, and learning rate
        string outFile;
        string trainFile;
        string EpochStr;
        int EpochNum;
        double LR;
        ifstream initfile;
        ifstream trainfile;
        ofstream outfile;

        cout << "Please input initial neural network file: ";
        cin >> inFile;

        initfile.open(inFile.c_str());
        if(!initfile) {
                cerr << "Error: Unable to open " << inFile << endl;   // Error checking
                return(-1);
        }

        cout << "Please input training file: ";
        cin >> trainFile;

        trainfile.open(trainFile.c_str());
        if(!trainfile) {
                cerr << "Error: Unable to open " << trainFile << endl;  // Error checking
                return(-1);
        }

        cout << "Please input file to write output of the trained neural network: ";
        cin >> outFile;

        outfile.open(outFile.c_str());
        if(!outfile) {
                cerr << "Error: Unable to open " << outFile << endl;  // Error checking
                return(-1);
        }

        cout << "Please set the number of epochs: ";
        cin >> EpochStr;
        EpochNum = stoi(EpochStr);

        cout << "Please specify a learning rate: ";
        cin >> LR;

        NeuralNet NN_train;
        NN_train.trainNN(initfile, trainfile, outfile, EpochNum, LR);

        initfile.close();
        trainfile.close();
        outfile.close();
        return 0;
}

int test() {
        string NNet;    //Declare variables for parsing files
        string outFile;
        string test;
        ifstream NNFile;
        ifstream testingFile;
        ofstream dataFile;

        cout << "Please input neural network file to test: ";
        cin >> NNet;

        NNFile.open(NNet.c_str());
        if(!NNFile) {
                cerr << "error: could not open " << NNet << endl;
                return(-1);
        }

        cout << "Please input a test set file to test neural network: ";
        cin >> test;

        testingFile.open(test.c_str());
        if(!testingFile) {
                cerr << "error: could not open " << test << endl;
                return(-1);
        }

        cout << "Please specify a results file to write output of the test to: ";
        cin >> outFile;

        dataFile.open(outFile.c_str());
        if(!dataFile) {
                cerr << "error: could not open " << outFile << endl;
                return(-1);
        }

        NeuralNet NN_test;
        NN_test.testNN(NNFile, testingFile, dataFile);

        NNFile.close();
        testingFile.close();
        dataFile.close();

        return 0;
}

int main() {
        int n;
        cout << "Choose to run training mode or testing mode:" << endl;
        cout << "Please press 0 to train." << endl;
        cout << "Please press 1 to test." << endl;

        cin >> n;

        switch (n)
        {
        case 0:
                train();
                break;
        case 1:
                test();
                break;
        default:
                cout << "Invalid mode; rerun program with a valid choice" << endl;
        }
        return 0;
}
