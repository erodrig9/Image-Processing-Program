#include <strings.h>
#include <math.h>
#include <stdio.h>

#include "GeneralizedListInterface.h"


void print (int x)
{
    fprintf(stderr, "%d, ", x);
}

void PrintLength (GenList *L)
{
    int length = Length(L);
    
    fprintf (stderr, "Length = %d --", Length(L));
}

int main (int argc, char **argv)
{
    GenList A, a, b;

    int p;

    Initialize (&A);
    Initialize (&a);
    Initialize (&b);

    AddAtom (2, 0, &a);
    AddAtom (3, 1, &a);
    AddAtom (5, 2, &a);
    
    AddAtom (4, 0, &b);
    AddAtom (6, 1, &b);
    AddAtom (7, 2, &b);
    AddAtom (8, 3, &b);
    
    //Append (&a, &b);

    AddList (&b, 0, &A);
    AddAtom (4, 0, &A);
    AddAtom (6, 1, &A);
    //AddList (&b, 1, &A);
    //AddAtom (10, 2, &A);

    Traverse(&A, print, PrintLength);
}




    