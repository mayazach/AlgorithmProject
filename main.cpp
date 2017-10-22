#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <sstream>
#include "curveList.h"
#include "distance.h"

using namespace std;

int main(int argc, char** argv){
	ifstream input; //input dataset
	ifstream query; //query dataset
	ofstream output; //output file
	bool stat = false,kflag = false,lflag = false,found = false;
	int k = 4, l = 5,i,j,dimension=2;
	int n=0; //number of curves in dataset
	char hash,func;
	CurveList mylist;//list to keep curves from input
	/** Variables for reading file input **/
	string in;
	double coord;
	stringstream ss;
	int start,end;
	Curve c;

	/**
		In this section, command line input is checked and argument values are assigned
		to variables. The number of arguments must be correct, and all the required 
		flags must be present and followed by their values. The user can give the arguments 
		in any order.
	**/
	
	if(argc > 16 || argc < 11){
		cout << "Wrong number of arguments" << endl;
		return 1;
	}
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-d")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			input.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-d flag and its value are required." << endl;
		return 1;
	}
	found = false;
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-q")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			query.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-q flag and its value are required." << endl;
		return 1;
	}
	found = false;
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-o")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			output.open(argv[i+1]);
			break;
		}
	}
	if(!found){
		cout << "-o flag and its value are required." << endl;
		return 1;
	}
	found = false;
	
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-function")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			if(!strcmp(argv[i+1],"DFT"))
				func = 'f';
			else if(!strcmp(argv[i+1],"DTW"))
				hash = 'w';
			else{
				cout << "-function value must be DFT or DTW" << endl;
			}
			break;
		}
	}
	if(!found){
		cout << "-function flag and its value are required." << endl;
		return 1;
	}
	found = false;
	
	for(i=1;i<(argc-1);i++){
		if(!strcmp(argv[i],"-hash")){
			found = true;
			if(argv[i+1][0] == '-'){
				cout << "Flag value missing." << endl;
				return 1;
			}
			if(!strcmp(argv[i+1],"classic"))
				hash = 'c';
			else if(!strcmp(argv[i+1],"probabilistic"))
				hash = 'p';
			else{
				cout << "-hash value must be classic or probabilistic" << endl;
			}
			break;
		}
	}
	if(!found){
		cout << "-hash flag and its value are required." << endl;
		return 1;
	}
	found = false;
	
	if(argc > 11){
		for(i=1;i<(argc-1);i++){
			if(!strcmp(argv[i],"-k")){
				kflag = true;
				if(argv[i+1][0] == '-'){
                    cout << "Flag value missing." << endl;
                    return 1;
                }
				k = atoi(argv[i+1]);
			}
			if(!strcmp(argv[i],"-L")){
				lflag = true;
				if(argv[i+1][0] == '-'){
                    cout << "Flag value missing." << endl;
                    return 0;
                }
				l = atoi(argv[i+1]);
            }
			if(!strcmp(argv[i],"-stats"))
				stat = true;
		}
		for(i=1;i<argc;i++){
			if(!strcmp(argv[i],"-stats"))
				stat = true;
		}
		if((!kflag) && (!lflag) && (!stat)){
			cout << "Unknown arguments." << endl;
			return 0;
		}
		if((argc == 16) && !(kflag && lflag && stat)){
			cout << "Optional flags must be -k, -L and -stats" << endl;
			return 0;
		}
	}
	/**
		Checking if input, output and query files opened correctly.
	**/
	if(input.fail() || query.fail() || output.fail()){
		cerr << "Error opening file" << endl;
		return 1;
	}

	/**
		In this section, the program reads the curves from the input file and adds them to 
		the list.
	**/
	
	c.dimension = dimension;
	while(!input.eof()){
		input >> c.id >> c.m;
		//cout << c.id << endl;
		c.points = new double*[c.m];
		for(i=0;i<c.m;i++)
			c.points[i] = new double[c.dimension];
		for(i=0;i<c.m;i++){
			getline(input,in,')');
			replace(in.begin(),in.end(),',',' ');
			replace(in.begin(),in.end(),'(',' ');
			ss.str("");
			ss << in;
			for(j=0;j<dimension;j++){
				ss >> coord;
				c.points[i][j] = coord;
			}
			ss.clear();
		}
		mylist.push(c);
	}
	n = mylist.getSize(); //get number of curves in dataset
	/** Closing files **/
	input.close();
	output.close();
	query.close();
}
