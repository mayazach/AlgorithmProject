#ifndef DFD-DTW
#define DFD-DTW
#include "curve.h"

double euclidean(double* p,double* q,int dim);

double dfd(Curve* p,Curve* q);

double dtw(Curve* p,Curve* q);

#endif