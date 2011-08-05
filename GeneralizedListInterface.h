#include "GeneralizedListTypes.h"

extern void InitializeGenList (GenList *L);
// Initializes the list to NULL. 

extern int ListLength (GenList *L);
// Returns the count of elements (atoms or lists) in the list pointed to by L
// For example, length of ((a, c), b, d, (e, k)) is 4.

extern int AddSubList (GenList *x, int position, GenList *L);
// Adds the list in x as the specified position in list L
// The value of "position" for the first element in the list is 0
// The list in x is set to NULL
// Return 0 to indicate error condition

extern int AddAtom (ItemType x, int position, GenList *L);
// Adds the atomic element x as the first element of list L
// The value of "position" for the first element in the list is 0
// Return 0 to indicate error condition

extern void TraverseGenList (GenList *L,  void (*VisitAtom) (ItemType), void (*VisitList) (GenList *));
// Generic travserse routine. The function VisitAtom is invoked for
// each atomic element. The function VisitList is invoked for
// each list.
