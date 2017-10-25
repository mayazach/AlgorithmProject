#include <iostream>
#include <string>
#include <limits>
#include "curveList.h"
#include "distance.h"

using namespace std;

void curvePrint(Curve curve){
	int i,j;
	cout << curve.id << endl << endl;
    if(curve.points != NULL){
    	for(i=0;i<curve.m-1;i++){
			cout << "(";
                for(j=0;j<curve.dimension-1;j++)
                    	cout << curve.points[i][j] << ", ";
			cout << curve.points[i][j] << "), ";
		}
        cout << "(";
        for(j=0;j<curve.dimension-1;j++)
            cout << curve.points[i][j] << ", ";
        cout << curve.points[i][j] << ")" << endl;
		cout << endl;
	}
}


CurveList::CurveList(){
	head = NULL;
	size = 0;
	//cout << "List just created" << endl;
};

CurveList::~CurveList(){
	//cout << "Destroying list" << endl;
	int i;
	while(head!=NULL){
		CurveNode *n = head->next;
		for(i=0;i<head->curve.m;i++)
			delete[]  head->curve.points[i];
		delete[] head->curve.points;
		delete head;
		head = n;
	}
};

int CurveList::getSize(){
	return size;
};

bool CurveList::isEmpty(){
	return (size==0);
};

void CurveList::push(Curve c){
	CurveNode *n = new CurveNode(c,head);
	head = n;
	size++;
};

Curve CurveList::remove(){
	CurveNode *n;
	n = head;
	head = head->next;
	Curve c = n->curve;
	size--;
	delete n;
	return c;
};

void CurveList::print(){
	CurveNode *n = head;
	while(n!=NULL){
		curvePrint(n->curve);
		n=n->next;
	}
};

Neighbor CurveList::minDist(Curve c,char func){
	Neighbor minNeighbor;
	double dist;
	CurveNode *n = head;
	Curve data;
	if(n!=NULL){
		data = n->curve;
		if(func == 'f'){
			minNeighbor.dist = dfd(&c,&(n->curve));
			minNeighbor.id = n->curve.id;
		}
		else{
			minNeighbor.dist = dtw(&c,&data);
			minNeighbor.id = n->curve.id;
		}
		n=n->next;
	}
	else{
		minNeighbor.dist = std::numeric_limits<double>::infinity();
		minNeighbor.id = "Not found";
	}
	while(n!=NULL){
		if(func == 'f')
			dist = dfd(&c,&(n->curve));
		else
			dist = dtw(&c,&data);
		if(dist < minNeighbor.dist){
			minNeighbor.dist = dist;
			minNeighbor.id = n->curve.id;
		}
		n=n->next;
	}
	return minNeighbor;
}
