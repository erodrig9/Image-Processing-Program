#include "HeapTreeTypes.h"

extern void InitializeHeap (HeapTree *T);
// This function will initialize the heap to NULL

extern int HeapEmpty (HeapTree *T); 
// This function returns 1 if Heap is empty and 0 if not.  It does not
// change the heap. It assumes that the heap has sbeen initialized
                
extern int HeapFull (HeapTree *T);
//Thus function returns 1 if the heap is full and 0 if not It does not
//change the heap. It assumes that the heap has been initialized.
                    
extern int HeapInsert (HeapItem Item, HeapTree *T, int (*Compare) (HeapItem X, HeapItem Y));   
// This function assumes that the heap (T) has been initialized. It
// inserts the given element into the heap provided it is not full. It
// return 1 if insertion has been successful and 0 otherwise. The
// function (*Compare) is used to compare two nodes. It should return
// -1 if X < Y, 0 if X==Y, and 1 if X > Y.

extern HeapItem HeapRemove (HeapTree *T, int (*Compare) (HeapItem X, HeapItem Y)); 
// This function assumes that the heap (T) has been initialized and is
// not empty. It removes from the heap and returns the element as the
// return value.  The function (*Compare) is used to compare two
// nodes. It should return -1 if X < Y, 0 if X==Y, and 1 if X > Y.
