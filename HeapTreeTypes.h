#include <strings.h>
#include <math.h>
#include <stdio.h>
#include "GeneralizedListTypes.h"

#ifndef Heap_Has_Been_Defined

#define  MAXCOUNT 100
 typedef  GenList  HeapItem;

 typedef  struct {
      int  Count;
      HeapItem Nodes[MAXCOUNT];
 } HeapTree;

#define Heap_Has_Been_Defined
#endif
