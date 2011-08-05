#include <strings.h>
#include <math.h>
#include <stdio.h>
#include "ImageInterface.h"

int ReadImage  (Image *I, char *FileName)
{
/*    Function:        Reads in the image I (graylevel or color) from File
      Preconditions:   None
      Postconditions:  All the image pixels are read in and the image
                     width and height recorded in the appropriate fields.
                     If the image is a gray level one, then all the three
                     color values of each pixel is set to intensity value of 
                     that pixel. It returns 1 for success and 0 for failure. */
  FILE *fp;
  int row, col, temp;
  char str[50];
  
  fp = fopen(FileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "\n Could not open file: %s\n", FileName);
    return (0);
  }
  fscanf(fp, "%s", str);
  if (strcmp (str, "P6") == 0 || strcmp (str, "P5") == 0) {
    
    fscanf(fp, "%d %d ", &(I->NCols), &(I->NRows));
    
    // Allocates space for a 2D array
    if (Initialize (I, I->NCols, I->NRows) == 0) {return(0);}

    fscanf(fp, "%d ", &temp) ;
    if (strcmp (str, "P6") == 0) {// Read in Color image pixels
      for (row=0; row < I->NRows; row++) {
	for (col=0; col < I->NCols; col++) {
	  I->Pixel[row][col].red   = (unsigned char) (getc(fp));
	  I->Pixel[row][col].green = (unsigned char) (getc(fp));
	  I->Pixel[row][col].blue  = (unsigned char) (getc(fp));
	}
      }
    }
    else {// real gray level image pixels
      for (row=0; row < I->NRows; row++) {
	for (col=0; col < I->NCols; col++) {
	  I->Pixel[row][col].red   = (unsigned char) (getc(fp));
	  I->Pixel[row][col].green =  I->Pixel[row][col].red;
	  I->Pixel[row][col].blue  =  I->Pixel[row][col].red;
	}
      }
    }
    fclose(fp);
    return (1);
  }
  else {
    fprintf(stderr, "\n Image in %s format is not PGM or PPM\n", FileName);
    return (0);
  }
}  

/* ------------------------------------------------------------------------*/

int SaveImage (Image *I, char *FileName, char Format[10])
{
/*  Function:        Write out the Image to the File in the format specific in Format
    Preconditions:   Format can be ``pgm'' or ``ppm''.
    Postconditions:  The written image file is in the specified format.
                     The case of the format string should not matter.
                     Returns 1 if successful, 0 if not. */

  FILE *fp;
  int row, col, gray;
  char str[50];
  
  fp = fopen(FileName, "w");
  if (fp == NULL) {
    fprintf(stderr, "\n Could not open file for writing: %s\n", FileName);
    return(0);
  }
  if (strcasecmp(Format, "PPM") == 0) {// save as PPM file.
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n255\n", I->NCols, I->NRows);
    for (row=0; row < I->NRows; row++) {
      for (col=0; col < I->NCols; col++) {
	putc((unsigned char) I->Pixel[row][col].red, fp);
	putc((unsigned char) I->Pixel[row][col].green, fp);
	putc((unsigned char) I->Pixel[row][col].blue, fp);
      }
    }
  }
  else if (strcasecmp(Format, "PGM") == 0) {// save as PGM file.
    fprintf(fp, "P5\n");
    fprintf(fp, "%d %d\n255\n", I->NCols, I->NRows);
    for (row=0; row < I->NRows; row++) {
      for (col=0; col < I->NCols; col++) {
	gray = (int) (I->Pixel[row][col].red+I->Pixel[row][col].green+I->Pixel[row][col].blue)/3;
	putc((unsigned char) gray, fp);
      }
    }
  }
  else {
     fprintf(stderr, "\n Not saved. Can only save in PPM or PGM formats\n");
     return (0);
  }
  fclose(fp);
  return (1);
}
  /* --------------------------------------------------------------------------*/
Color GetPixel (Image *I, int Col, int Row)
/*    Function:   Returns the pixel value at (Row, Col)
                  Returns zero, i.e. red = green = blue = 0,
                   if the (Row, Col) falls outside the image
    Preconditions:   None
    Postconditions:  Should not change the Image*/
{
  Color Black;
  if  (InBounds (I, Col, Row) == 1) {
    return(I->Pixel[Row][Col]);
  }
  else {
    Black.red = 0; Black.blue = 0; Black.green = 0;
    return(Black);
  }
	
}
/* -------------------------------------------------------------------------*/
 int SetPixel (Image *I, Color Value, int Col, int Row)
/*    Function:   Sets the pixel value at (Row, Col) to  Value
    Preconditions:   None
    Postconditions:  Should change the Image only at the specific pixel location. 
                  Returns 0 if the (Row, Col) falls outside the image
                  and return 1 otherwise*/
{
  if (InBounds (I, Col, Row) == 1) {
    I->Pixel[Row][Col].red = Value.red;
    I->Pixel[Row][Col].green = Value.green;
    I->Pixel[Row][Col].blue = Value.blue;
    return (1);
  }
  else
    return (0);
}
/* -------------------------------------------------------------------------*/
int GetSize (Image *I, int *NCol, int *NRow)
/*  Function:   Returns the total number of Rows and Columns in the Image 
    Preconditions:   None
    Postconditions: The values are returned through the procedure parameters.
                    It returns 1 for success and 0 for failure.*/
{
  *NRow = I->NRows;
  *NCol = I->NCols;
}
/* -------------------------------------------------------------------------*/
 int Initialize (Image *I, int NCol, int NRow)
/*  Function: Sets the total number of Rows and Columns in the Image 
    Preconditions:   None
    Postconditions: The values are returned through the Image parameter. 
                    It returns 1 for success and 0 for failure */
{
  int row;
  I->NRows = NRow; I->NCols = NCol;
  I->Pixel = (Color ** ) malloc( ( I->NRows ) * sizeof(Color *));
  if (I->Pixel == NULL) {return(0);}
  for(row = 0; row < I->NRows; row++ ) { 
      I->Pixel[row] = (Color *)malloc((I->NCols ) * sizeof(Color)); 
      if (I->Pixel[row] == NULL) {return(0);}
  }
  return(1);
}
/*-----------------------------------------------------------------------*/
int Free (Image *I)
/*  Function: Frees the memory used to store the pixels for an image
    Preconditions:   None
    Postconditions: Pixel pointers, if any are set to NULL and the image
                    size, i.e. number of rows and columns are set to 0.*/
{
  int row;
  for(row = 0; row < I->NRows; row++ ) { 
    free(I->Pixel[row]);
  }
  free(I->Pixel);
  I->NRows = 0; I->NCols = 0; I->Pixel = NULL;
  return(1);
}
/* ------------------------------------------------------------------------*/
int InBounds (Image *I, int Col, int Row)
/*  Function: Returns 1 if the pixel (Row, Col) is inside the image
                 boundary, return 0, otherwise
    Preconditions: Image should be initialized
    Postconditions: None */
{
  if ((Row >= 0) && (Col >= 0) && (Row < I->NRows) && (Col < I->NCols)) {
    return (1);
  }
  else
    return (0);
}

/* -----------------------------------------------------------------------*/
