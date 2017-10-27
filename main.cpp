#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <limits>
#include <ctime>
#include "curveList.h"
#include "distance.h"
#include "randomnum.h"
#include "hash.h"
#include "gridcurves.h"

using namespace std;

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
	int rn = 10; //number of max R-neighbors to return;
	string* rNeighbors = new string[rn]; //R-neighbors ids
	int rIndex;
	/** Variables for reading file input **/
	string in;
	double coord,d = 0.0005,r=0,tMin,tMax,tAvg,tTrue;
	double* maxDif;
	double* minDif;
	double* avgDif;
	stringstream ss;
	int start,end,tablesize;
	Curve c;
	Neighbor trueNeighbor,lshNeighbor,tempNeighbor;
	hashTable** lTables;
	float seconds;
	clock_t t1,t2,tt1,tt2,tr1,tr2;

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
	
	input >> c.id;
	if(c.id == "3")
		dimension = 3;
	else if(c.id == "4")
		dimension = 4;
	else{
		input >> c.m;
		c.points = new double*[c.m];
		for(i=0;i<c.m;i++)
			c.points[i] = new double[dimension];
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
	c.dimension = dimension;
	while(!input.eof()){
		input >> c.id >> c.m;
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
	tablesize = n/16; //Number of buckets in each hash table
	
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
	
	
	if(!stat){
		
		/**
			The code in this section runs if the stats flag is not given. The L hash tables 
			are created once. Each curve from the input file is put in each hash table, at the 
			index calculated using its grid curves. True nearest neighbor is found by searching 
			all the buckets in the first hashtable and LSH nearest neighbor is found using the 
			LSH algorithm. Results are printed to cout and to output file.
		**/
		
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
		
		while(!mylist.isEmpty()){
			c = mylist.remove();
			for(i=0;i<l;i++){
				hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
				position = hash_function(hash_value,tablesize);
				hash_insert(c,position,lTables[i]);
			}
		}
		
		while(!queryList.isEmpty()){
			for(i=0;i<rn;i++)
				rNeighbors[i] = " ";
			rIndex = 0;
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
				rIndex = lTables[i]->kadoi[position].dataList->findRNeighbors(rNeighbors,rIndex,rn,r,c,func);
				if(tempNeighbor.dist < lshNeighbor.dist){
					lshNeighbor.dist = tempNeighbor.dist;
					lshNeighbor.id = tempNeighbor.id;
				}
			}
			cout << "True distance: " << trueNeighbor.dist << endl;
			cout << "True nneighbor: " << trueNeighbor.id << endl;
			cout << "LSH distance: " << lshNeighbor.dist << endl;
			cout << "LSH nneighbor: " << lshNeighbor.id << endl;
			sort(rNeighbors,rNeighbors + rIndex );
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
			output << "distanceTrue: " << trueNeighbor.dist << endl;
			output << "R-near neighbors: " << endl;
			int i =0;
			while(i<rIndex){
				output << rNeighbors[i] << endl;
				i++;
			}
			output << endl;
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
		/**
			The code in this section runs if the stats flag is not given. The hash tables are created 
			100 times. True distance is calculated only the first time and LSH distance once in each 
			repetition. Results are printed to cout and to the output file.
			
			Two lists each are used to store the input and query curves. Each time a curve is removed 
			from one list and put in the hash tables, it is pushed to the other. The process is then 
			repeated, moving the curves back to the first list while making the next hash tables. All 
			this happens 50 times.
		**/
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
			lshNeighbor.dist = std::numeric_limits<double>::infinity();
			for(j=0;j<n;j++){
				lshNeighbors[j].dist = std::numeric_limits<double>::infinity();
			}
			cout << i << endl;
			t1 = clock();
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
			count = 0;
			while(!queryList.isEmpty()){
				c = queryList.remove();
				bQueryList.push(c);
				cout << "Id: " << c.id << endl;
				//find true distance
				if(i==0){
					tr1 = clock();
					for(j=0;j<tablesize;j++){
						tempNeighbor = lTables[0]->kadoi[j].dataList->minDist(c,func);
						if(tempNeighbor.dist < trueNeighbors[count].dist){
							trueNeighbors[count].dist = tempNeighbor.dist;
						}
					}
					tr2 = clock();
				}
				if(i==0)
						tt1 = clock();
				for(j=0;j<l;j++){
					hash_value = gridify(k,curve_t[0],c,d,hash,dimension);
					position = hash_function(hash_value,tablesize);
					tempNeighbor = lTables[j]->kadoi[position].dataList->minDist(c,func);
					if(tempNeighbor.dist < lshNeighbors[count].dist){
						lshNeighbors[count].dist = tempNeighbor.dist;
					}
				}
				if(i==0)
						tt2 = clock();
				count++;
			}
			
			for(j=0;j<l;j++)
				destroy_hashTable(lTables[j]);
			delete [] lTables;
			
			for(j=0;j<l;j++)
				delete [] curve_t[j];
			delete [] curve_t;
			
			t2 = clock();
			seconds = ((float)t2 - (float)t1) / CLOCKS_PER_SEC;
			if(i==0){
				tTrue = seconds - (((float)tt2 - (float)tt1) / CLOCKS_PER_SEC);
				seconds -= (((float)tr2 - (float)tr1) / CLOCKS_PER_SEC);
			}
			if(seconds > tMax)
				tMax = seconds;
			if(seconds < tMin)
				tMin = seconds;
			tAvg += seconds/100;
			
			
			for(j=0;j<n;j++){
				if(abs(trueNeighbors[j].dist- lshNeighbors[j].dist) > maxDif[j])
					maxDif[j] = abs(trueNeighbors[j].dist- lshNeighbors[j].dist);
				if(abs(trueNeighbors[j].dist- lshNeighbors[j].dist) < minDif[j])
					minDif[j] = abs(trueNeighbors[j].dist- lshNeighbors[j].dist);
				avgDif[j] += abs(trueNeighbors[j].dist- lshNeighbors[j].dist)/100;
			}
			
			for(j=0;j<n;j++){
				lshNeighbors[j].dist = std::numeric_limits<double>::infinity();
			}
			
			t1 = clock();
			
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
			
			count = n-1;
			while(!bQueryList.isEmpty()){
				lshNeighbor.dist = std::numeric_limits<double>::infinity();
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
			
			t2 = clock();
			seconds = ((float)t2 - (float)t1) / CLOCKS_PER_SEC;
			if(seconds > tMax)
				tMax = seconds;
			if(seconds < tMin)
				tMin = seconds;
			tAvg += seconds/100;
			
			for(j=0;j<n;j++){
				if(abs(trueNeighbors[j].dist- lshNeighbors[j].dist) > maxDif[j])
					maxDif[j] = abs(trueNeighbors[j].dist- lshNeighbors[j].dist);
				if(abs(trueNeighbors[j].dist- lshNeighbors[j].dist) < minDif[j])
					minDif[j] = abs(trueNeighbors[j].dist- lshNeighbors[j].dist);
				avgDif[j] += abs(trueNeighbors[j].dist- lshNeighbors[j].dist)/100;
			}
		}
		count = 0;
		cout << "RESULTS" << endl << endl;
		while(!queryList.isEmpty()){
			c = queryList.remove();
			cout << "Id: " << c.id << endl;
			cout << maxDif[count] << endl;
			cout << minDif[count] << endl;
			cout << avgDif[count] << endl;
			cout << tMax << endl;
			cout << tMin << endl;
			cout << tAvg << endl;
			
			output << "Query: " << c.id << endl;
			if(func == 'f')
				output << "DistanceFunction: DFT" << endl;
			else
				output << "DistanceFunction: DTW" << endl;
			if(hash == 'c')
				output << "HahFunction: Classic" << endl;
			else
				output << "HahFunction: Probabilistic" << endl;
			output << "|minDistanceLSH – distanceTrue|: " << minDif[count] << endl;
			output << "|maxDistanceLSH – distanceTrue|: " << minDif[count] << endl;
			output << "|avgDistanceLSH – distanceTrue|:" << avgDif[count] << endl;
			output << "tLSHmin: " << tMin << endl;
			output << "tLSHmax: " << tMax << endl;
			output << "tLSHavg: " << tAvg << endl;
			output << "tTrue: " << tTrue << endl << endl;
			for(i=0;i<c.m;i++)
				delete [] c.points[i];
			delete [] c.points;
			count++;
		}
		
		//Cleanup
		delete [] rNeighbors;
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