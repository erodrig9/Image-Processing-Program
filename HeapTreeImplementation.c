#include <strings.h>
#include <math.h>
#include <stdio.h>
#include "HeapTreeInterface.h"
#include "ProcessImage.h"

void InitializeHeap (HeapTree *T)
{
	// Initialize count to 0
	T->Count = 0;
		
// This function will initialize the heap to NULL
}

int HeapEmpty (HeapTree *T)
{
	//if empty return 1 else return 0
	if (T->Count = 0){
		return 1;
	}
	else{
		return 0;
	}
// This function returns 1 if Heap is empty and 0 if not.  It does not
// change the heap. It assumes that the heap has sbeen initialized
}

int HeapFull (HeapTree *T)
{
	//if full return 1 else return 0
	if (T->Count >= MAXCOUNT){
		return 1;
	}
	else{
		return 0;
	}
}

int HeapInsert (HeapItem Item, HeapTree *T, int (*Compare) (HeapItem X, HeapItem Y))
{
    if(!HeapFull(T))
    {
        int ChildLocation, ParentLocation, compare;

        (T->Count)++;
        ChildLocation = T->Count;
        ParentLocation = ChildLocation/2;

        while (ParentLocation != 0)
        {
            //if(Item <= T->Nodes[ParentLocation])                        //compare
            // -1 if X < Y, 0 if X==Y, and 1 if X > Y.
            compare = (*Compare)(Item, T->Nodes[ParentLocation]);
            if(compare == -1 || compare == 0) 
            {
                T->Nodes[ChildLocation] = Item;
                return 1;
            }
            else
            {
                T->Nodes[ChildLocation] = T->Nodes[ParentLocation];
                ChildLocation = ParentLocation;
                ParentLocation = ParentLocation/2;
            }
        }
        
		T->Nodes[ChildLocation] = Item;
        return 1;
    }
    else return(0); 
}   

HeapItem HeapRemove (HeapTree *T, int (*Compare) (HeapItem X, HeapItem Y))
{
    int CurrentLocation, ChildLocation, compare;
    HeapItem Delete, Replace;

    if(HeapEmpty(T))return;
    
    Delete = T->Nodes[1];   //Delete 1 or 0?
    Replace = T->Nodes[T->Count-1];
    (T->Count)--;

    CurrentLocation = 1;
    ChildLocation = 2*CurrentLocation;
    
    while(ChildLocation <= T->Count)
    {
        //Set Child to location of larger than child of current location
        if(ChildLocation < T->Count)
        {
            //if(T->Nodes[ChildLocation+1] > T->Nodes[ChildLocation])
            // -1 if X < Y, 0 if X==Y, and 1 if X > Y.
            compare = (*Compare)(T->Nodes[ChildLocation+1], T->Nodes[ChildLocation]);
            if(compare == 1)
            {   // Compare function
                ChildLocation++;
            }
        }
        // -1 if X < Y, 0 if X==Y, and 1 if X > Y.
        //if(T->Nodes[ChildLocation] <= Replace) 
        compare = (*Compare)(T->Nodes[ChildLocation] , Replace);
        if(compare == -1 || compare == 0)
        {                         
            T->Nodes[CurrentLocation] = Replace;
            return(Delete);
        }
        else
        {
            T->Nodes[CurrentLocation] =T->Nodes[ChildLocation];
            CurrentLocation = ChildLocation;
            ChildLocation = 2*CurrentLocation;
        }
    }
    
    T->Nodes[CurrentLocation] = Replace;
    
    return(Delete);
}

   
