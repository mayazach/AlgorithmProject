#include <iostream>
#include <string>
#include "curveList.h"

using namespace std;

void curvePrint(Curve curve){
	int i,j;
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
	}
}


CurveList::CurveList(){
	head = NULL;
	size = 0;
	cout << "List just created" << endl;
};

CurveList::~CurveList(){
	while(head!=NULL){
		CurveNode *n = head->next;
		delete head;
		head = n;
	}
	cout << "List destroyed" << endl;
};

int CurveList::getSize(){
	return size;
};

bool CurveList::isEmpty(){
	return (size==0);
};

void CurveList::push(Curve& c){
	CurveNode *n = new CurveNode(c,head);
	head = n;
	size++;
};

Curve& CurveList::remove(){
	CurveNode *n;
	n = head;
	head = head->next;
	Curve& c = n->curve;
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

int main(){
	int i;
	CurveList mylist;
	Curve curve;
	curve.m = 6;
	curve.dimension = 2;
	curve.points = new double*[curve.m];
	for(i=0;i<curve.m;i++)
		curve.points[i] = new double[2];
	curve.points[0][0] = -6.4227999999999996;
	curve.points[0][1] =  53.288000000000004;
	curve.points[1][0] = -6.4257099999999996;
	curve.points[1][1] = 53.289299999999997;
	curve.points[2][0] = -6.4268099999999997;
	curve.points[2][1] = 53.290300000000002;
	curve.points[3][0] = -6.4268099999999997;
	curve.points[3][1] = 53.290300000000002;
	curve.points[4][0] = -6.4271699999999994;
	curve.points[4][1] = 53.290599999999998;
	curve.points[5][0] = -6.4271699999999994;
	curve.points[5][1] = 53.290599999999998;
	curvePrint(curve);
	cout << mylist.isEmpty() << endl;

	mylist.push(curve);
	cout << mylist.isEmpty() << endl;
	mylist.remove();
	cout << mylist.isEmpty() << endl;

	for(i=0;i<curve.m;i++)
		delete[]  curve.points[i];
	delete[] curve.points;
}

