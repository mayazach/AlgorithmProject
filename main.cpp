#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <limits>
#include "curveList.h"
#include "distance.h"
#include "randomnum.h"
#include "hash.h"

using namespace std;

int gridify(int k,double* tvalues,Curve c,double d, char hash,int dimension);

int hash_classic(double* g,int gsize);

int hash_lsh(double* g, int gsize);

int main(int argc, char** argv){
	ifstream input; //input dataset
	ifstream query; //query dataset
	ofstream output; //output file
	bool stat = false,kflag = false,lflag = false,found = false;
	int k = 2, l = 3,i,j,count,dimension=2,hash_value,position;
	int n=0; //number of curves in dataset
	char hash,func;
	CurveList mylist;//list to keep curves from input
	CurveList queryList;//list to keep curves from query file
	CurveList backlist;
	CurveList bQueryList;
	double** curve_t; //t values for creating grid curves
	/** Variables for reading file input **/
	string in;
	double coord,d = 0.0005,r=0,tMin,tMax,tAvg;
	double* maxDif;
	double* minDif;
	double* avgDif;
	stringstream ss;
	int start,end,tablesize;
	Curve c;
	Neighbor trueNeighbor,lshNeighbor,tempNeighbor;
	hashTable** lTables;

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
				func = 'w';
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
	tablesize = n/16;
	
	/**
		In this section, the program reads the curves from the query file saves them
	**/
	
	query >> c.id >> r;
	while(!query.eof()){
		query >> c.id >> c.m;
		c.points = new double*[c.m];
		for(i=0;i<c.m;i++)
			c.points[i] = new double[c.dimension];
		for(i=0;i<c.m;i++){
			getline(query,in,')');
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
		queryList.push(c);
	}
	//queryList.print();
	
	if(!stat){
		/** Creating hash tables **/
		lTables = new hashTable*[l];
		for(i=0;i<l;i++)
			lTables[i] = create_hashTable(tablesize);
		
		curve_t = new double*[l];
		for(i=0;i<l;i++)
			curve_t[i] = new double[k];
		
		for(i=0;i<l;i++){
			for(j=0;j<k;j++)
				curve_t[i][j] = ranf(d);
		}
		
		//mylist.print();
		
		while(!mylist.isEmpty()){
			c = mylist.remove();
			for(i=0;i<l;i++){
				hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
				position = hash_function(hash_value,tablesize);
				hash_insert(c,position,lTables[i]);
			}
		}
		
		while(!queryList.isEmpty()){
			trueNeighbor.dist = std::numeric_limits<double>::infinity();
			lshNeighbor.dist = std::numeric_limits<double>::infinity();
			c = queryList.remove();
			cout << "Id: " << c.id << endl;
			//find true distance
			for(i=0;i<tablesize;i++){
				tempNeighbor = lTables[0]->kadoi[i].dataList->minDist(c,func);
				if(tempNeighbor.dist < trueNeighbor.dist){
					trueNeighbor.dist = tempNeighbor.dist;
					trueNeighbor.id = tempNeighbor.id;
				}
			}
			for(i=0;i<l;i++){
				hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
				position = hash_function(hash_value,tablesize);
				tempNeighbor = lTables[i]->kadoi[position].dataList->minDist(c,func);
				if(tempNeighbor.dist < lshNeighbor.dist){
					lshNeighbor.dist = tempNeighbor.dist;
					lshNeighbor.id = tempNeighbor.id;
				}
			}
			cout << "True distance: " << trueNeighbor.dist << endl;
			cout << "True nneighbor: " << trueNeighbor.id << endl;
			cout << "LSH distance: " << lshNeighbor.dist << endl;
			cout << "LSH nneighbor: " << lshNeighbor.id << endl;
			output << "Query: " << c.id << endl;
			if(func == 'f')
				output << "DistanceFunction: DFT" << endl;
			else
				output << "DistanceFunction: DTW" << endl;
			if(hash == 'c')
				output << "HahFunction: Classic" << endl;
			else
				output << "HahFunction: Probabilistic" << endl;
			output << "LSH Nearest neighbor: " << lshNeighbor.id << endl;
			output << "True Nearest neighbor: " << trueNeighbor.id << endl;
			output << "distanceLSH: " << lshNeighbor.dist << endl;
			output << "distanceTrue: " << trueNeighbor.dist << endl << endl;
			//find lsh distance
			
			//find r neighbors
		}
		
		/**
			Cleanup
		**/
		
		for(i=0;i<l;i++)
			destroy_hashTable(lTables[i]);
		delete [] lTables;
		
		for(i=0;i<l;i++)
			delete [] curve_t[i];
		delete [] curve_t;
	
	}
	else{
		n = queryList.getSize();
		Neighbor* trueNeighbors = new Neighbor[n];
		Neighbor* lshNeighbors = new Neighbor[n];
		maxDif = new double[n];
		minDif = new double[n];
		avgDif = new double[n];
		for(i=0;i<n;i++){
			trueNeighbors[i].dist = std::numeric_limits<double>::infinity();
		}
		for(i=0;i<n;i++){
			maxDif[i] = 0;
		}
		for(i=0;i<n;i++){
			avgDif[i] = 0;
		}
		for(i=0;i<n;i++){
			minDif[i] = std::numeric_limits<double>::infinity();
		}
		tMax = 0;
		tMin = std::numeric_limits<double>::infinity();
		tAvg = 0;
		trueNeighbor.dist = std::numeric_limits<double>::infinity();
		for(i=0;i<50;i++){
			cout << i << endl;
			lTables = new hashTable*[l];
			for(j=0;j<l;j++)
				lTables[j] = create_hashTable(tablesize);
			
			curve_t = new double*[l];
			for(j=0;j<l;j++)
				curve_t[j] = new double[k];
			
			for(j=0;j<l;j++)
				for(count=0;count<k;count++)
					curve_t[j][count] = ranf(d);
				
			while(!mylist.isEmpty()){
				c = mylist.remove();
				backlist.push(c);
				for(j=0;j<l;j++){
					hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
					position = hash_function(hash_value,tablesize);
					hash_insert(c,position,lTables[j]);
				}
			}
			
			while(!queryList.isEmpty()){
				lshNeighbor.dist = std::numeric_limits<double>::infinity();
				for(j=0;j<n;j++){
					lshNeighbors[j].dist = std::numeric_limits<double>::infinity();
				}
				count = 0;
				c = queryList.remove();
				bQueryList.push(c);
				cout << "Id: " << c.id << endl;
				//find true distance
				if(i==0){
					for(j=0;j<tablesize;j++){
						tempNeighbor = lTables[0]->kadoi[j].dataList->minDist(c,func);
						if(tempNeighbor.dist < trueNeighbors[count].dist){
							trueNeighbors[count].dist = tempNeighbor.dist;
						}
					}
				}
				for(j=0;j<l;j++){
					hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
					position = hash_function(hash_value,tablesize);
					tempNeighbor = lTables[j]->kadoi[position].dataList->minDist(c,func);
					if(tempNeighbor.dist < lshNeighbors[count].dist){
						lshNeighbors[count].dist = tempNeighbor.dist;
					}
				}
				count++;
			}
			
			for(j=0;j<l;j++)
				destroy_hashTable(lTables[j]);
			delete [] lTables;
			
			for(j=0;j<l;j++)
				delete [] curve_t[j];
			delete [] curve_t;
			
			lTables = new hashTable*[l];
			for(j=0;j<l;j++)
				lTables[j] = create_hashTable(tablesize);
			
			curve_t = new double*[l];
			for(j=0;j<l;j++)
				curve_t[j] = new double[k];
			
			for(j=0;j<l;j++)
				for(count=0;count<k;count++)
					curve_t[j][count] = ranf(d);
				
			while(!backlist.isEmpty()){
				c = backlist.remove();
				mylist.push(c);
				for(j=0;j<l;j++){
					hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
					position = hash_function(hash_value,tablesize);
					hash_insert(c,position,lTables[j]);
				}
			}
			
			while(!bQueryList.isEmpty()){
				lshNeighbor.dist = std::numeric_limits<double>::infinity();
				for(j=0;j<n;j++){
					lshNeighbors[j].dist = std::numeric_limits<double>::infinity();
				}
				count = n-1;
				c = bQueryList.remove();
				queryList.push(c);
				cout << "Id: " << c.id << endl;
				for(j=0;j<l;j++){
					hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
					position = hash_function(hash_value,tablesize);
					tempNeighbor = lTables[j]->kadoi[position].dataList->minDist(c,func);
					if(tempNeighbor.dist < lshNeighbors[count].dist){
						lshNeighbors[count].dist = tempNeighbor.dist;
					}
				}
				count--;
			}
			
			for(j=0;j<l;j++)
				destroy_hashTable(lTables[j]);
			delete [] lTables;
			
			for(j=0;j<l;j++)
				delete [] curve_t[j];
			delete [] curve_t;
		}
		delete [] trueNeighbors;
		delete [] lshNeighbors;
		delete [] maxDif;
		delete [] minDif;
		delete [] avgDif;
	}
	
	/** Closing files **/
	input.close();
	output.close();
	query.close();
}

int gridify(int k,double* tvalues,Curve c,double d, char hash,int dimension){
	Curve* gridcurves = new Curve[k];
	//curvePrint(c);
	int size = c.m;
	int i,j,n,m;
	int duplicates = 0,gsize=0;
	int result;
	double round;
	bool same;
	
	//Preparing g vertice
	double* g = new double[size*k*dimension];
	for(i=0;i<(size*k*dimension);i++)
		g[i] = 0;
		
	//Creating grid curves
	for(i=0;i<k;i++){
		gridcurves[i].m = size;
		gridcurves[i].dimension = c.dimension;
		gridcurves[i].points = new double*[size];
		gridcurves[i].id = c.id;
		for(j=0;j<size;j++)
			gridcurves[i].points[j] = new double[dimension];
		for(j=0;j<size;j++)
			for(n=0;n<dimension;n++){
				round = fmod(c.points[j][n],d);
				if(round <= d/2){
					gridcurves[i].points[j][n] = c.points[j][n] + tvalues[i] - round;
				}
				else{
					gridcurves[i].points[j][n] = c.points[j][n] + tvalues[i] - round + d;
				}
			}
	}
		
	//curvePrint(gridcurves[0]);
	
	//TODO: move this to remove_duplicates function
	for(i=0;i<k;i++){
		duplicates = 0;
		for(j=1;j<size-duplicates;j++){
			same = true;
			for(n=0;n<dimension;n++)
				if(gridcurves[i].points[j][n] != gridcurves[i].points[j-1][n])
					same = false;
			if(same){
				duplicates++;
				for(n=j;n<size;n++){
					for(m=0;m<dimension;m++)
						gridcurves[i].points[n-1][m] = gridcurves[i].points[n][m];
				}
				for(n=0;n<dimension;n++)
					gridcurves[i].points[size-duplicates][n] = 0;
				j--;
			}
		}
		for(j=0;j<size-duplicates;j++)
			for(n=0;n<dimension;n++){
				g[gsize] = gridcurves[i].points[j][n];
				gsize++;
			}
	}
	
	
	if(hash == 'c')
		result = hash_classic(g,gsize);
	else
		result = hash_lsh(g,gsize);
	
	//cleanup
	for(i=0;i<k;i++){
		for(j=0;j<size;j++)
			delete [] gridcurves[i].points[j];
		delete [] gridcurves[i].points;
	}
	delete [] gridcurves;
	delete [] g;
	return result;
}

int hash_classic(double* g,int gsize){
	int* r = new int[gsize];
        int i;
        double sum=0;
        for(i=0;i<gsize;i++)
            r[i]=random_num();
        for(i=0;i<gsize;i++)
            sum=sum+r[i]*g[i];
        /*metatroph tou sum/g[i] se int*/	
	delete [] r;
	return (int)sum;
}

int hash_lsh(double* g, int gsize){
	int i,j;
	int w=4,kvec=3;
	int* v = new int[gsize];
	int* r = new int[gsize];
	double* t = new double[kvec];
	int sum = 0;
	
	for(i=0;i<gsize;i++)
        r[i]=random_num();
	for(i=0;i<kvec;i++)
		t[i]=ranf(w);
	for(i=0;i<gsize;i++)
		v[i]= marsaglia_num();
	
	for(i=0;i<gsize;i++)
		for(j=0;j<kvec;j++)
			sum = (int) ((g[i]*v[i] + t[j])/w);
		
	
	delete [] v;
	delete [] r;
	delete [] t;
	return abs(sum);
}
