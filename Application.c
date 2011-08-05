#include <strings.h>
#include <math.h>
#include <stdio.h>
#include "ImageInterface.h"
#include "QueueInterface.h"
#include "StackInterface.h"
#include "GeneralizedListInterface.h"
#include "ProcessImage.h"
#include "HeapTreeInterface.h"


/*-----------------------------------------------------------------------*/
int main (int argc, char **argv)
{
  Image InputImage, SampleColorPatch, ThresholdedImage, ComponentImage;
  float MeanColor[3], StdDevColor[3], ThresholdValue;
  char FileName[100];
  HeapTree Components;
  int SearchChoice, ComponentSize, NR, NC;
  

  if (argc != 7) {
    fprintf(stderr, "\nUsage: %s sample-color-file input-image-file output-file-name       thresholdvalue ComponentSize SearchType\n", argv[0]);
    exit(1);
  }
  TestADTs ();
  
 
  ReadImage (&InputImage, argv[2]);
  ReadImage (&SampleColorPatch, argv[1]);  
  ThresholdValue = atoi(argv[4]);
  ComponentSize = atoi(argv[5]);
  SearchChoice = atoi(argv[6]);

  fprintf (stderr, "\n Computing color statistics based on image in %s", argv[1]);
  ComputeRGBStatistics (&SampleColorPatch, MeanColor, StdDevColor);
  fprintf (stderr, "\n Mean: (%f %f %f), StdDev = (%f %f %f)", MeanColor[0], MeanColor[1], MeanColor[2], StdDevColor[0], StdDevColor[1], StdDevColor[2]);
  
  
  fprintf (stderr, "\n Thresholding image in %s...", argv[2]);
  ColorThreshold (&InputImage, MeanColor, StdDevColor, ThresholdValue, &ThresholdedImage);
  fprintf (stderr, "...done");   

  //SaveImage(&ThresholdedImage, argv[3], "PPM");

  GetSize (&InputImage, &NC, &NR);
  Initialize (&ComponentImage, NC, NR);
  SaveComponents (&ThresholdedImage, &ComponentImage, ComponentSize, SearchChoice);
  SaveImage(&ComponentImage, argv[3], "PPM");
}

/******************************************************************************************/


int TestADTs ()
{

  Queue Q; 
  Stack S;
  Pixel P;  
  GenList A, a, b;
  int i;
  
  void print (Pixel x) {
    fprintf(stderr, "(%d %d), ", x.X, x.Y);
  }
  
  void PrintLength (GenList *L) {
    fprintf (stderr, "\n(%d", ListLength(L));
  }

  //Test Queues
  InitializeQueue(&Q);
  fprintf(stderr, "\n Testing Queue: ");
  for (i=1; i < 10; i++) {P.X = i; P.Y = i; Insert (P, &Q);}
  for (i=1; i < 10; i++) {Remove (&Q, &P); fprintf(stderr, "(%d %d)", P.X, P.Y);}
  
  InitializeStack(&S);
  fprintf(stderr, "\n Testing Stack: ");
  for (i=1; i < 10; i++) {P.X = i; P.Y = i; Push (P, &S);}
  for (i=1; i < 10; i++) {Pop (&S, &P); fprintf(stderr, "(%d %d)", P.X, P.Y);}
  
  
  fprintf(stderr, "\n Testing Gen List: ");
  InitializeGenList (&A); 
  InitializeGenList  (&a);
  InitializeGenList  (&b);

  P.X = 2; P.Y = 2; AddAtom (P, 0, &a);
  P.X = 3; P.Y = 3; AddAtom (P, 1, &a);
  P.X = 5; P.Y = 5; AddAtom (P, 2, &a);
  fprintf(stderr, "\n Length of list a is %d", ListLength(&a));

  P.X = 4; P.Y = 4;  AddAtom (P, 0, &b);
  P.X = 6; P.Y = 6;   AddAtom (P, 1, &b);
  P.X = 7; P.Y = 7;   AddAtom (P, 2, &b);
  P.X = 8; P.Y = 8;   AddAtom (P, 3, &b);
  fprintf(stderr, "\n Length of list b is %d", ListLength(&b));

  AddSubList (&a, 0, &A);
  AddSubList (&b, 1, &A);
  P.X = 10; P.Y = 10;    AddAtom (P, 2, &A);
  fprintf(stderr, "\n Length of list A is %d", ListLength(&A));
  fprintf(stderr, "\n Printing List A");
  TraverseGenList (&A, print, PrintLength);

}
