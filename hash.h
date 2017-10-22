
struct bins{
  CurveList* dataList;
  int numData;

};

struct hashTable
{
   int positions;
   bins* kadoi;
};




int hash_function(int*,int,int);
hashTable* create_hashTable(int);
void destroy_hashTable(hashTable*);
void hash_insert(Curve*,int,hashTable*);
void hash_print(hashTable*);
CurveList* hash_take_insertions(int,hashTable*);
