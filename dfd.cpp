#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include "curve.h"
using namespace std;

typedef struct Curve curve;

/*typedef struct Curve{
	int m;
	double** points;
}curve;
*/
/*p->simeio ths kabylhs a,q->simeio ths kabylhs b*/
double euclidean(double* p,double* q,int dim)
{
cout<<"euclidean begins\n";
double d=0;
int i;
for(i=0;i<dim;i++)
{
  d=d+pow((q[i]-p[i]),2);
  cout<<"p["<<i<<"]"<<p[i]<<"\n";
  cout<<"q["<<i<<"]"<<q[i]<<"\n";
  
}
d=sqrt(d);
cout<<"d="<<d<<"\n";
cout<<"end_euclidean\n";
return d;
}
/*synartish dfd*/
/*double dfd(curve p,curve q,int m1,int m2,int dim ,int i,int j)*/
/*m1->arithmos simeiwn ths p*/
/*m2->arithmos simeiwn ths q*/

/*arxika kaleitai me i=m1 kai j=m2*/
double dfd(curve* p,curve* q)
{
/*dim->arithmos simeiwn ths */
cout<<"dfd begins!!\n";
int m1=p->m;
int m2=q->m;
/*cout<<"ok\n";*/
double max=0;
double min=0; 
int i;
int j;
double result;
int dim=p->dimension;
/*
double** C;
((m1+1)*sizeof(double*));
for(i=0;i<=m1;i++)
{
   C[i]=(double*)malloc((m2+1)*sizeof(double));


}
*/
/*cout<<"bike\n";*/
double** C=new double*[(m1+1)];
for(i=0;i<=m1;i++)
{
   C[i]=new double[(m2+1)];

}
/*anaprosarmozw tous deiktes gia na ksekinane apo 0*/
/* ?? */
/*h kalytera kanw pinaka me diastaseis m1+1,m2+1*/
/*gia na mhn allaksoun oi deiktes*/
/*kai arxikopoiw me 0 auta pou einai sthn 0 grammh h sth 0 sthlh*/
for(i=0;i<=m1;i++)
{
   for(j=0;j<=m2;j++)
   {
   C[i][j]=max;

   }
}

for(i=1;i<=m1;i++)
{
for(j=1;j<=m2;j++)
{
 
  if((i==1)&&(j==1))
  {
  max=euclidean(p->points[i-1],q->points[j-1],dim); 

  }
  if((j>1)&&(i==1))
  {
    /*find max*/
    max=C[1][j-1];
    if(euclidean(p->points[i-1],q->points[j-1],dim)>max)
    {
    max=euclidean(p->points[i-1],q->points[j-1],dim);
    }

  } 
  if((i>1)&&(j==1))
  { 
    /*find max*/
    max=C[i-1][1];
    if(euclidean(p->points[i-1],q->points[j-1],dim)>max)
    {
    max=euclidean(p->points[i-1],q->points[j-1],dim);
    }
  }
  if((i>1)&&(j>1))
  {
    /*find min*/
    min=C[i-1][j];
    cout<<"min"<<min<<"\n";
    if(C[i-1][j-1]<min)
    {
    min=C[i-1][j-1];
    cout<<"min"<<min<<"\n";
    }
    if(C[i][j-1]<min)
    {
    min=C[i][j-1];
    cout<<"min"<<min<<"\n";
    }
    cout<<"min"<<min<<"\n";
    /*find max*/
    max=min;
    if(euclidean(p->points[i-1],q->points[j-1],dim)>max)
    {
    max=euclidean(p->points[i-1],q->points[j-1],dim);
    }
    /*C[i][j]=max;*/   
    
    
   
  }
  cout<<"i="<<i;
  cout<<"\n";
  cout<<"j="<<j;
  C[i][j]=max;
  cout<<"\n";
  cout<<"value stored in C["<<i<<"]"<<"["<<j<<"]="<<C[i][j]<<"\n";
  cout<<"\n";
  cout<<"\n";
}
}
cout<<"C[m1][m2]:"<<C[m1][m2]<<"\n";
result=C[m1][m2];
/*apodesmeysh xwrou*/
/*for(i=0;i<curve.m;i++)
		delete[]  curve.points[i];
	delete[] curve.points;
}
*/
for(i=0;i<=m1;i++)
{
    delete[] C[i];
}
delete[] C;
cout<<"end_dfd\n";
return result;
}
/*dokimasrikh main pou apla tha kalei th synartish*/
int main()
{
int i;
int j;
/*int dim=2;*/


/*dimiourgia curve p*/
curve* p=new curve();
p->m=2;
p->dimension=2;
p->points=new double*[(p->m)];
for(i=0;i<(p->m);i++)
{
  p->points[i]=new double[p->dimension];
  
}

/*dimiourgia curve q*/
curve* q =new curve();
q->m=2;
q->dimension=2;
q->points=new double*[(q->m)];
for(i=0;i<(q->m);i++)
{
   q->points[i]=new double[p->dimension];


}

/*1	7	(-6.4227999999999996, 53.288000000000004), (-6.4257099999999996, 53.289299999999997), (-6.4268099999999997, 53.290300000000002), (-6.4268099999999997, 53.290300000000002), (-6.4268099999999997, 53.290300000000002), (-6.4271699999999994, 53.290599999999998), (-6.4271699999999994, 53.290599999999998)*/

/*dinodai times sta simeia twn cabilwn*/
p->points[0][0]=0;
p->points[0][1]=1;
p->points[1][0]=1;
p->points[1][1]=1;

q->points[0][0]=0;
q->points[0][1]=2;
q->points[1][0]=1;
q->points[1][1]=4;

cout<<"klish dfd\n";
double result;
result=dfd(p,q);
cout<<"result:"<<result<<"\n";


/*apodesmeysh xwrou*/
for(i=0;i<(p->m);i++)
{
    delete[] p->points[i];   

}
delete[] p->points;
delete p;
for(i=0;i<(q->m);i++)
{
   delete[] q->points[i];

}
delete[] q->points;
delete q;
}



