#ifndef HASHTABLE_H
#define HASHTABLE_H

  #include <iostream>
  #include "DefragRunner.h"
  #include "mynew.h"

  using namespace std;

  class HashNode //keeps track of the block and it's assosiated position
  {
      public:
      DiskBlock* block;
      int pos;

      HashNode() : block(NULL), pos(0){}
      HashNode(DiskBlock* b, int p) : block(b), pos(p){}
      DiskBlock* getBlock(){return block;}
      int getPos(){return pos;}
      void setPos(int p){pos = p;}
  };

  class HashTable //no need for a count because the table never rehashes
  {
      public:
      HashNode arr[200000];
      int size;

      HashTable();
      HashTable(int nSize);
      HashNode find(int pos);
      int getSize(){return size;}
      void insert(HashNode node);
      void print();
  };


#endif
