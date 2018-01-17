#include "HashTable.h"
#include <iostream>


HashTable::HashTable() : size(0)
{

} //HashTable()

HashTable::HashTable(int nSize) :  size(nSize)
{

} //HashTable(int nsize)

HashNode HashTable::find(int pos)
{
    //finds the HashNode at position pos, returns an empty node if not found
    int indexToFind = pos % size;
    int collisionPos = indexToFind;

    for(int i = 0; i < size; i++)
    {

      if(arr[collisionPos].getPos() == pos) //you find the node
      {
        // 'lazy delete'
        HashNode returnNode = arr[collisionPos]; // save the position to a new node
        arr[collisionPos].setPos(0); //reset our node's position to 0. this is our 'lazy delete' as the function never looks at anything pointing to 0

        return returnNode;
      } //if

      collisionPos = indexToFind + i;

    } //for

    return HashNode(); //return the empty constructor if not found.

} // find(int pos)

void HashTable::insert(HashNode node)
{
  //inserts into the arr using linear probing.


  int indexToInsert = node.getPos() % size;
  int i = 0;
  int collisionPos = indexToInsert;

  while(arr[collisionPos].getPos() != 0) // while there are collisons
  {

    if(arr[collisionPos].getPos() == 0) // there is an empty position
    {

      break;

    } //if

    collisionPos = indexToInsert + i;
    i++;
  } //while

  arr[collisionPos] = node;
} //insert(HashNode node)

void HashTable::print()
{
   //prints the hashtable
   for(int i = 0; i < size; i++)
   {

     cout << i << " | ";

     if(arr[i].getPos() != 0) //there is an element at that postion
     {

       //arr[i].print();
       cout << "\n";

     } //if
     else //there is no element
     {

       cout << "\n";

     } //else
   } //for


} //void print()
