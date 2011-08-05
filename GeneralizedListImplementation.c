#include "GeneralizedListTypes.h"

/*---------------------------------------------------------------------*/
void InitializeGenList (GenList *L)
// Initializes the list to NULL. 
{
  L->head = (GenListNode *) malloc(sizeof(GenListNode)); //dummy header
  L->head->next = NULL;
  L->head->AtomP = -1;
  L->count = 0;
  //fprintf(stderr, "\n %d", L->count);
}
/*---------------------------------------------------------------------*/
int ListLength (GenList *L)
// Returns the count of elements (atoms or lists) in the list pointed to by L
// For example, length of ((a, c), b, d, (e, k)) is 4.
{
  int Count = 0;
  GenListNode *Current;

  Current = L->head->next;
  while (Current != NULL) {
    Count++;
    Current = Current->next;
  }
  //fprintf(stderr, "\n %d %d", Count, L->count);
 return(Count);
}

/*---------------------------------------------------------------------*/
int AddSubList (GenList *x, int position, GenList *L) 
// Adds the list in x as the specified position in list L
// The value of "position" for the first element in the list is 0
// The list in x is set to NULL
// Return 0 to indicate error condition
{
  GenListNode *NewNode, *Previous;
  int count;
  
  if ((position < 0) || (position > L->count)) return 0; //position outside the list
  NewNode = (GenListNode *) malloc (sizeof(GenListNode));
  NewNode->AtomP = 0;
  NewNode->SubNode.List = x->head;
  
  Previous = L->head; 
  count = 0;
  while (count != position) {
    Previous = Previous->next;
    count++;
  }
  NewNode->next = Previous->next;
  Previous->next = NewNode;
  L->count++;
}
    
/*---------------------------------------------------------------------*/
int AddAtom (ItemType x, int position, GenList *L)
// Adds the atomic element x as the first element of list L
// The value of "position" for the first element in the list is 0
// Return 0 to indicate error condition
{
  GenListNode *NewNode, *Previous;
  int count;
  
  if ((position < 0) || (position > L->count)) return 0; //position outside the list
  NewNode = (GenListNode *) malloc (sizeof(GenListNode));
  NewNode->AtomP = 1;
  NewNode->SubNode.Item = x;
  
  Previous = L->head; 
  count = 0;
  while (count != position) {
    Previous = Previous->next;
    count++;
  }
  NewNode->next = Previous->next;
  Previous->next = NewNode;
  L->count++;
}
    
/*---------------------------------------------------------------------*/
 void TraverseGenList (GenList *L,  void (*VisitAtom) (ItemType), void (*VisitList) (GenList *))
// Generic travserse routine. The function VisitAtom is invoked for
// each atomic element. The function VisitList is invoked for
// each list.
{
  GenList TempList;
  GenListNode *Current;
  
  Current = L->head->next;
  while (Current != NULL) {
    if (Current->AtomP == 1) {
      (*VisitAtom) (Current->SubNode.Item);
    }
    else if (Current->AtomP == 0) {
      InitializeGenList (&TempList);
      TempList.head->next = Current->SubNode.List;
      TempList.count = ListLength (&TempList);
     (*VisitList) (&TempList);
     TraverseGenList (&TempList, VisitAtom, VisitList);
    }
    Current = Current->next;
  }
}


