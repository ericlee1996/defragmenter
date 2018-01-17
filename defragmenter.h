// Author Sean Davis
#ifndef defragmenterH
  #define defragmenterH

#include "mynew.h"
#include "DefragRunner.h"

class Defragmenter
{
public:
  Defragmenter(DiskDrive *diskDrive);
  int findNextEmpty(DiskDrive* diskDrive, int& fullest);

}; // class Defragmenter

#endif
