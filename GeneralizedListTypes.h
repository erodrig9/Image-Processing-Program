#include <strings.h>
#include <math.h>
#include <stdio.h>

#ifndef Pixel_Has_Been_Defined
   typedef struct pixel {
     int X, Y;
   } Pixel;
   typedef Pixel ItemType;
#define Pixel_Has_Been_Defined
#endif

#ifndef GeneralizedList_Has_Been_Defined
   typedef struct generalizedlist {
      struct generalizedlist *next;
      int AtomP; // 1 indicates atomic element, 
                 // 0 indicates list in the SubNode
      union subnode {
        ItemType Item;
        struct generalizedlist *List;
      } SubNode;
   } GenListNode;
   
  typedef struct list {
     GenListNode *head;
     int count;
  } GenList;

#define GeneralizedList_Has_Been_Defined
#endif

