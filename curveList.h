#ifndef CURVELIST
#define CURVELIST
#include <iostream>
#include "curve.h"

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
	~CurveList();
	bool isEmpty();
	int getSize();
	void push(Curve c);
	Curve remove();
	void print();
};

#endif
