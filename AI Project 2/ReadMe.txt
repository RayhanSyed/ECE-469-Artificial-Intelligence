Rayhan Syed
AI Project 2
Professor Sable
12-9-19

Building and running program:

I ran and tested this program in ubuntu 18.04LTS with gcc and 2011 C++ features.
Within this folder, there are 3 datasets as well as my project which is composed of the make file, main.cpp file, NN.h file, and NN.cpp file.
To compile, you just need to run make and the make file should carry the rest. The make file will make an NN.exe which can be run via ./NN.exe.

Each of the dataset folders contains a text file matching one of the respective descriptions with a prefix matching the relvant dataset:
 A) inital neural net,
 B) Training Set,
 C) Trained NN,
 D) Testing Set,
 E) Testing Results

Dataset Decription:

For my personal dataset, Ray Lee helped me by directing me to the one that he used previously, which is UC Irvine's iris flower dataset.
The dataset consits of 120 training examples as well as 30 testing examples. I used matlab to make gaussian noise with average 0 and 0.5 standard deviation for the  edge weights of each node for the initial neural network.
The URL of the dataset is: https://archive.ics.uci.edu/ml/datasets/iris

With this dataset, the neural network tries to classify between three diffirent flowers: Virginica, Setosa, and Versicolor.
 Since there are three different classes, I used 3 output nodes.
Additionally, there are 4 features in the dataset: petal length, petal width, sepal length, and sepal width, so I used 4 input nodes.
After doing a bunch of research on stack exchange in which I got many rules of thumb, of which multiple were conflicting, I settled on 5 as the best mix between rules such as between the # input and the # number of output, and 2/3 the # of input + the # of output, and less than twice the # of input layer neurons. All three of this gave different ranges and so I settled with 5 which I best thought fit my numbers. as I did not think it would cause over or underfitting. I suspect these rules of thumb are for way more complex single neural networks.

I found that a learning rate 0.04 and 125 epochs lead to pretty good results and so that is what I used when making the trained neural net and results file.
