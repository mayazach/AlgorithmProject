#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
/*#include "hash.h"*/
#include "curveList.h"
/*h curveList xreiazetai gia ta bins*/
/*ousiastika h curveList tha exei ta gridcurves pou adistoixoun se ena */
/*prepei na yparxei adistoixish se akeraio->ginetai me lista akeraiwn*/
typedef struct Curve curve;

using namespace std;

typedef struct bin{
  CurveList* dataList;
  int numData;

}bins;

typedef struct hash_table
{
   int positions;
   bins* kadoi;
}hashTable;


/*??*/
/*k grid curves-k dianysmata*/
/*gia kathe dianysma pairnw mia synartish h*/
/*sth synexeia ypologizw to f=ri*h(p)+...*/
/*??*/


/*hash function->pairnei orisma ta h,ypologizei ta r kia th thes*/



int hash_function(int* h,int k,int TableSize)
{
   /*apo thn curve tha prokypsoun gia kathe simeio */
   /*to h exei ftiaxtei na dinetai san orisma */
   /*mexri na vrethei o tropos ypologismou tou*/


   /*to M einai gnwsto*/
   /*M=pow(2,32)-5;*/
   int M=7;

   /*dimiourgia pinaka r*/
   int i;
   int* r;
   int f=0;
   int hashPosition;
   r=new int[k];
   for(i=0;i<k;i++)
   {
     /*to r tha ftiaxtei na pairnei tyxaies times */
     r[i]=10;
   }
   for(i=0;i<k;i++) 
   {
     f=f+r[i]*h[i];
   }
   hashPosition=f%M;
   hashPosition=hashPosition%TableSize;
   delete[] r;
   return hashPosition;



}


/*kataskeyh hashtable*/
hashTable* create_hashTable(int ar_kadwn)
{
   hashTable* hashT=new hashTable();
   hashT->positions=ar_kadwn;
   /*dimiourgia ar_kadwn*/
   int i;
   
   
   hashT->kadoi=new bins[ar_kadwn];
   for(i=0;i<hashT->positions;i++) 
   hashT->kadoi[i].dataList=new CurveList();   
   /**/
   cout<<"hashTable created"<<"\n";
   return hashT;
}




/*diagrafh hashtable*/
void destroy_hashTable(hashTable* hashT)
{
   /*diagrafh thesewn*/
   /*diagrafh tou pinaka*/
   int i;
   /*gia kathe bin diagrafh ths curveList*/
    for(i=0;i<hashT->positions;i++)
    delete hashT->kadoi[i].dataList;
   /*destroy_bin*/
   delete[] hashT->kadoi;
   
   delete hashT;

}


/*eisagwgh neou curve*/
void hash_insert(curve* input,int thesh,hashTable* hashT)
{
   /*eisagwsgh sth sto hashT*/
   /*eisagwgh sthn curveList*/
   hashT->kadoi[thesh].dataList->push(*input);
   cout<<"insertion completed"<<"\n";

      
}








int main()
{
   /*hash_function(h,,);*/
   int* h=new int[3];


   /*curve dokimi*/
   int i;

   curve* p=new curve();
   p->m=2;
   p->dimension=2;
   p->points=new double*[(p->m)];
   for(i=0;i<(p->m);i++)
   {
      p->points[i]=new double[p->dimension];
  
   }


   p->points[0][0]=0;
   p->points[0][1]=1;
   p->points[1][0]=1;
   p->points[1][1]=1;


   /*dokimastikes times gia to h*/
   h[0]=1;
   h[1]=2;
   h[2]=3;

   int TableSize=5;
   /*int Tablesize=n/2;*/
   int ar_kadwn=5;

   /*dimiourgia hashtable*/

   hashTable* hashT=create_hashTable(ar_kadwn);
   int thesh=hash_function(h,3,TableSize);
   cout<<"thesh-dokimh"<<thesh<<"\n";

   /*eisagwgh tou curve sth thesh 1 tou hashtable*/
   hash_insert(p,1,hashT);



   /*apodesmeudh xwrou hashtable*/
   /*estw oti h kabulh p tha bei sth thesh 1*/
   destroy_hashTable(hashT);
   /*(h,3,hash_function);*/
   delete[] h;

   cout<<"hashTable deleted"<<"\n";

    /*apodesmeysh xwrou*/
   delete p;

  cout<<"ok\n";
  /*hash_function(h,k,TableSize);
  hash_function(h,k,TableSize);
  */
}



/**/









