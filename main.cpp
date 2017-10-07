#include <iostream>
#include <fstream>
#include <cstdlib>
#include "curve.h"

using namespace std;

int main(int argc, char** argv){
	ifstream input;
	ifstream query;
	ofstream output;
	bool stat,kflag,lflag;
	int k = 4, l = 5;

	if(argc > 16 || argc < 10){
		cout << "Wrong number of arguments" << endl;
		return 1;
	}

	Curve a;
	a.m = 12;
	cout << l << endl;
}
