#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "HashTable.h"
#include "QueueAr.h"

Defragmenter::Defragmenter(DiskDrive *diskDrive)
{
  int tracker[500001];
  HashTable ram(200000);
  Queue<DiskBlock*> q(1000);
  int ramCount = 0;
  int fullest = diskDrive -> getCapacity();

  for(int i = diskDrive -> getCapacity(); i > 0 ; i--)
  {
    if(diskDrive -> FAT[i] == 1)
    {
      fullest--;
    }
  }


  int insertPos = 2; // the first position is at 2 and you want to insert from there


  for (int i = 0; i < diskDrive->getNumFiles(); i++) // for every directory
  {

    int blockPos = diskDrive->directory[i].getFirstBlockID();
    diskDrive -> directory[i].setFirstBlockID(insertPos);
    int size = diskDrive -> directory[i].getSize();
    for(int j = 0 ; j < size; j++) // 1 indicates the end of the directory
    {

      if (blockPos < insertPos) // this indicates that the file has been overwritten
      {                         // insertPos is incremented whenever diskDrive is written

        if (tracker[blockPos] < 0) // when tracker[blockPos] is in ram
        {
          HashNode found = ram.find(blockPos);
          //diskDrive -> FAT[blockPos] = 0;
          q.enqueue(found.getBlock());
          blockPos = found.getBlock() -> getNext();
        }

        else if(tracker[blockPos] >= 0)
        {

          int newPos = tracker[blockPos];
          DiskBlock* block = diskDrive -> readDiskBlock(newPos);
          q.enqueue(block);
          tracker[newPos] = 0; // lazy delete
          tracker[blockPos] = 0; // lazy delete
          diskDrive -> FAT[newPos] = 0;
          if(newPos > fullest)
          {
            fullest =  newPos;
          }
          blockPos = block -> getNext(); // incrementing blockPos
          continue;
        }


        continue; // once we have found blockPos in the ram we are out of here
      }

      DiskBlock* dskBlock = diskDrive->readDiskBlock(blockPos);

      q.enqueue(dskBlock); // **enqueue right here to save to the queue and
                          // find what has been enqueued by dequeueying
      diskDrive -> FAT[blockPos] = 0; // once we have enqueued we are going to immediately dequeue it
      if(fullest < blockPos)
      {
        fullest = blockPos;
      }
      blockPos = dskBlock->getNext(); //increment blockPos
    }


    while(!q.isEmpty())
    {

      if (diskDrive->FAT[insertPos] == 1) // 1 indicates that the file is not empty
      {                                   // we want to swap the original file with the new file
        if(tracker[insertPos] != 0) // not empty (moved downward or moved twice)
        {

          int newPos = tracker[insertPos]; // if moved then linked to the original position
          HashNode node(diskDrive -> readDiskBlock(insertPos), newPos); // newPos = original position
          ram.insert(node);
          tracker[tracker[insertPos]] = -1; // value in the original value
        }
        else if(tracker[insertPos] == 0)// empty
        {

          HashNode node(diskDrive->readDiskBlock(insertPos), insertPos); // reading the new file
          ram.insert(node);   // inserting to the ram so stuff can be taken out from the ram later
          ramCount++;
          tracker[insertPos] = -1;
        }

      }
      DiskBlock* dequeued = q.dequeue(); // dequeued diskBlock node
      diskDrive->writeDiskBlock(dequeued, insertPos);
      delete(dequeued); // delete the block to free memory
      insertPos++; // everytime diskDrive is written
    }

    if(ramCount >= 6000)
    {

      for(int i = 0; i < ram.getSize(); i++)
      {
        if(ram.arr[i].getPos() != 0)
        {
          int index = findNextEmpty(diskDrive, fullest);
          diskDrive -> writeDiskBlock(ram.arr[i].getBlock(), index);
          tracker[ram.arr[i].getPos()] = index; // original position linked to new position
          tracker[index] = ram.arr[i].getPos(); // new position linked to original position
          delete(ram.arr[i].block);
          ram.arr[i].pos = 0;

        }
      }
      ramCount = 0;
    }


}

} // Defragmenter()

int Defragmenter::findNextEmpty(DiskDrive* diskDrive, int& fullest)
{
  int index = fullest;
  while(diskDrive -> FAT[index] == 1)
  {
    index--;
  }
  diskDrive -> FAT[index] = 1;
  fullest = index; // guarantees FAT[index] will always be the fullest
  return index;
}
