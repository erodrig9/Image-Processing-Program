#include <strings.h>
#include <math.h>
#include <stdio.h>
#include "ImageInterface.h"
#include "QueueInterface.h"
#include "StackInterface.h"
#include "GeneralizedListInterface.h"
#include "ProcessImage.h"
#include "HeapTreeInterface.h"



//-----------------------------------------------------------------------------

int ComputeRGBStatistics (Image *Input, float *Mean, float *Std)
{ // This function computes the mean and the standard deviation of 
  // RGB channels in the color image Input. The computed statistics
  // are returned in the float arrays pointed to by Mean and Std.
  // The values corrresponding to the red, green, and blue channels
  // are stored at indices 0, 1, and 2, respectively.

  int row, col, NR, NC, N, i;
  Color pixel;
  
  GetSize (Input, &NC, &NR);
  Mean[0] = Mean[1] = Mean[2] = 0;
  Std[0] = Std[1] = Std[2] = 0;
  
  for (row=0; row < NR; row++) {
    for (col=0; col < NC; col++) {
      pixel = GetPixel(Input, col, row);      
      Mean[0] += pixel.red;
      Mean[1] += pixel.green;
      Mean[2] += pixel.blue;
      Std[0] += pixel.red*pixel.red;
      Std[1] += pixel.green*pixel.green;
      Std[2] += pixel.blue*pixel.blue;
      
    }
  }
  // Normalize the values and compute the standard deviation.
  N = NR * NC;
  for (i=0; i < 3; i++) {
    Mean[i] = Mean[i]/N;
    Std[i] = sqrt (Std[i]/N - Mean[i]*Mean[i]);
  }
  return (1);
}
/*---------------------------------------------------------------------*/
int ColorThreshold (Image *Input, float *Mean, float *Std, float threshold, Image *Out)
{// Thresholds the input color image based on how many standard deviations
 // the color is away from the mean color. The "threshold" parameter specifies
 // this required distance. The output image, Out, is not assumed to have the
 // size setup. The foreground in the Out image is black and the background is white.

  int row, col, NR, NC, N;
  Color pixel, black, white;
  float Distance;

  
  black.red = 0; black.blue = 0; black.green = 0;
  white.red = 255; white.blue = 255; white.green = 255;
  GetSize (Input, &NC, &NR);
  Initialize (Out, NC, NR);

  for (row=0; row < NR; row++) {
    for (col=0; col < NC; col++) {
      pixel = GetPixel(Input, col, row);
      Distance = (pixel.red - Mean[0])*(pixel.red - Mean[0])/(Std[0]*Std[0]);
      Distance += (pixel.green - Mean[1])*(pixel.green - Mean[1])/(Std[1]*Std[1]);
      Distance += (pixel.blue - Mean[2])*(pixel.blue - Mean[2])/(Std[2]*Std[2]);
      if(Distance < threshold*threshold)  
	SetPixel(Out, black, col, row);
      else SetPixel(Out, white, col, row);
    }
  }
}
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
int Breadth_First_Search (Image *Input_Image, GenList *L, Pixel P)
{
  Queue Q; //Queue of (x, y) coordinates of a pixel
  int px, py, i, j;
  Color one, pColor;
  Pixel R, nR;

  one.red = 1; one.blue = 1; one.green = 1;
  InitializeQueue(&Q);
  Insert (P, &Q);
  SetPixel (Input_Image, one, P.X, P.Y);
  while (QueueEmpty(&Q) == 0) {
    Remove(&Q, &R);
    //fprintf(stderr, "(%d %d)", P.X, P.Y);
    AddAtom (R, 0, L);
    for (i=-1; i <=1; i++)
      for (j=-1; j <=1; j++)
	if ((i!= 0) || (j!=0)) {//ignore rx, and ry (central pixel)
	  nR.X = R.X+i;  nR.Y = R.Y+j; 
	  if (InBounds (Input_Image, nR.X, nR.Y) ==1) {
	    //fprintf(stderr, "[%d %d]", nR.X, nR.Y);
	    pColor = GetPixel (Input_Image, nR.X, nR.Y);
	    if ((pColor.red == 0) && (pColor.blue == 0) && (pColor.green == 0)){
	      SetPixel (Input_Image, one, nR.X, nR.Y); /* A label to indicate that the pixel has been enqueued*/
	      Insert(nR, &Q);
	    }
	  }
	}
  }
}


/*---------------------------------------------------------------------*/
int Depth_First_Search (Image *Input_Image, GenList *L, Pixel P)
{
  Stack S; //Stack of (x, y) coordinates of a pixel
  int px, py, i, j;
  Color one, pColor;
  Pixel R, nR;

  one.red = 1; one.blue = 1; one.green = 1;
  InitializeStack (&S);
  Push (P, &S);
  SetPixel (Input_Image, one, P.X, P.Y);
  while (StackEmpty(&S) == 0) {
    Pop(&S, &R);
    //fprintf(stderr, "(%d %d)", P.X, P.Y);
    AddAtom (R, 0, L);
    for (i=-1; i <=1; i++)
      for (j=-1; j <=1; j++)
	if ((i!= 0) || (j!=0)) {//ignore rx, and ry (central pixel)
	  nR.X = R.X+i;  nR.Y = R.Y+j; 
	  if (InBounds (Input_Image, nR.X, nR.Y) ==1) {
	    //fprintf(stderr, "[%d %d]", nR.X, nR.Y);
	    pColor = GetPixel (Input_Image, nR.X, nR.Y);
	    if ((pColor.red == 0) && (pColor.blue == 0) && (pColor.green == 0)){
	      SetPixel (Input_Image, one, nR.X, nR.Y); /* A label to indicate that the pixel has been enqueued*/
	      Push(nR, &S);
	    }
	  }
	}
  }
}

/*---------------------------------------------------------------------*/
// The second argument of this function need to be changed to a heap
int ExtractComponents (Image *Input, HeapTree *L, int Choice)
{// The foreground in the Input image is black and the background is white.
 // It extracts individual components and stores the pixels in a generalized
  // list. The foreground pixels in Input are changed to a gray level
  // of value 1 from 0.
  // if Choice == 0, then depth first search is used and if Choice == 1 then
  // breadth first search is used.

  int row, col, NR, NC;
  Color pColor;
  Pixel P;
  GenList NewL;
  
  int CompareNodes(HeapItem X, HeapItem Y)
  {
    int CountX = 0, CountY = 0;
    GenListNode *TempNodeX;
	GenListNode *TempNodeY;
	
	TempNodeX = X.head->next;
    while(TempNodeX != NULL){
        CountX++;
        TempNodeX = TempNodeX->next;
    }
    TempNodeY = Y.head->next;
    while(TempNodeY != NULL){
        CountY++;
        TempNodeY = TempNodeY->next;
    }
    if(CountX < CountY) return(-1);
    if(CountX == CountY) return(0);
    if(CountX > CountY) return(1);
    return(2);
  }
  GetSize (Input, &NC, &NR);

  fprintf(stderr, "\n Extracting Components...");
  for (row=0; row < NR; row++) {
    for (col=0; col < NC; col++) {
      pColor = GetPixel(Input, col, row);
      if ((pColor.red == 0) && (pColor.blue == 0) && (pColor.green == 0)){
	    P.X = col; P.Y=row;
	    InitializeGenList (&NewL);
	    if (Choice == 0) {
	      fprintf(stderr, "\n Component %d is", L->Count);
	      Depth_First_Search (Input, &NewL, P);
	      fprintf(stderr, "of size %d pixels.", ListLength (&NewL)); 
	      //AddSubList (&NewL, 0, L);
          HeapInsert (NewL, L, CompareNodes);
	    }
	    if (Choice == 1) {
	      fprintf(stderr, "\n Component %d is", L->Count);
	      Breadth_First_Search (Input, &NewL, P);
	      fprintf(stderr, "of size %d pixels.", ListLength (&NewL)); 
	      //AddSubList (&NewL, 0, L); // This would be changed to inserting into a heap
          HeapInsert (NewL, L, CompareNodes);  
	    }
      }
    }
  }
  fprintf(stderr, "done"); 
}

/*-----------------------------------------------------------------------*/
// This function will change significantly to make it work a Heap of GenLists.
int SaveComponents (Image *Input, Image *Output, int Component_Size, int SearchChoice) 
//Takes an black and white Input image and marks the components above
// Component_Size with color values.
{
  int Flag, row, col, NC, NR, i;
  Color Label, white;
  GenListNode *TempNode;
  GenList CompList;
  Pixel P;
  HeapTree Components;      //HeapTree
  
  white.red = 255; white.blue = 255; white.green = 255;
  GetSize (Output, &NC, &NR);
  for (row=0; row < NR; row++) {
    for (col=0; col < NC; col++) {
      SetPixel (Output, white, col, row);
    }
  }
  InitializeHeap(&Components);
  ExtractComponents (Input, &Components, SearchChoice);
  Flag = 0; 
  Label.red = 100; Label.blue = 1; Label.green = 1;
  //TraverseGenList (&Components, ProcessAtom, ProcessList);
  fprintf(stderr, "\n %d \n", Components.Count);
  for(i = 1; i < Components.Count; i++){
	InitializeGenList (&CompList);
	CompList.head->next = Components.Nodes[i].head;
    CompList.count = ListLength (&CompList);

	if (ListLength (&CompList) > Component_Size) {
      //fprintf(stderr, "\n Saving component of size %d", ListLength (&CompList));
      Flag = 1;
      //Label.red++;  Label.blue++; Label.green++;
      Label.red = (5*(1+Label.red))%255;
      Label.green = (5*Label.red)%255; 
      Label.blue = (10*Label.red)%255;
    }
	else Flag = 0;
	fprintf(stderr, "245 -- 245");
    TempNode = CompList.head->next;
    while (TempNode != NULL) {
	   if (Flag == 1) {
          //fprintf(stderr,"(%d %d)", P.X, P.Y);
		  P = TempNode->SubNode.Item;
          SetPixel (Output, Label, P.X, P.Y);
          //fprintf(stderr,"-");
	   }
	   TempNode = TempNode->next;
	   fprintf(stderr, "%d", i);
    }
  }
  fprintf(stderr, "FINSIHED!");
}  
  
  /*void ProcessList (GenList *L) {
    if (ListLength (L) > Component_Size) {
      fprintf(stderr, "\n Saving component of size %d", ListLength (L));
      Flag = 1;
      //Label.red++;  Label.blue++; Label.green++;
      Label.red = (5*(1+Label.red))%255;
      Label.green = (5*Label.red)%255; 
      Label.blue = (10*Label.red)%255;
      
    }
    else Flag = 0;
  }*/
  /*void ProcessAtom (Pixel P) {
    if (Flag == 1) {// the flag is set by previous call to ProcessList
      //fprintf(stderr,"(%d %d)", P.X, P.Y);
      SetPixel (Output, Label, P.X, P.Y);
      //fprintf(stderr,"-");
      
    }
  }*/


