#ifndef CURVELIST
#define CURVELIST
#include <iostream>
#include "curve.h"

/**
	Takes a curve and prints out its vector.
**/
void curvePrint(Curve curve);

class CurveList{

	struct CurveNode{
		Curve curve;
		CurveNode *next;
		CurveNode(Curve c,CurveNode *point=NULL):curve(c),next(point){}
	};
	CurveNode *head;
	int size;
public:
	CurveList();
	~CurveList(); //deletes the list, and frees all the curves' vectors
	bool isEmpty();
	int getSize();
	void push(Curve c); //add curve to the front of the list
	Curve remove(); //removes curve from the front of the list
	void print(); //prints out the vectors of all curves in the list
};

#endif
