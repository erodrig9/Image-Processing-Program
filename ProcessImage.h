#include "HeapTreeInterface.h"
#include "ImageInterface.h"

int ComputeRGBStatistics (Image *Input, float *Mean, float *Std);
int ColorThreshold (Image *Input, float *Mean, float *Std, float threshold, Image *Out);
int Breadth_First_Search (Image *Input_Image, GenList *L, Pixel P);
int Depth_First_Search (Image *Input_Image, GenList *L, Pixel P);
int Depth_First_Search (Image *Input_Image, GenList *L, Pixel P);
int ExtractComponents (Image *Input, HeapTree *L, int Choice);
int SaveComponents (Image *Input, Image *Output, int Component_Size, int SearchChoice);
//int CompareNodes(HeapItem X, HeapItem Y);
