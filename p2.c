#include"vision_utilities.h"
#include<stdio.h>
#include<stdlib.h>


//void testConvert();
void printArr(int** toPrint, int nRows, int nCols);
void convert(Image* im, int** grid, int* labels, int nRows, int nCols);

int main(int nargin, char** argv){
  if(nargin != 3){
    printf("Usage: p2 <binary img> <output file>");
    return -1;
  }

  Image* im;
  im = (Image*) malloc(sizeof(Image));
  readImage(im, argv[1]);
  
  int i, j;
  int nRows, nCols;
  nRows = getNRows(im);
  nCols = getNCols(im);

  int** grid = (int**) calloc(sizeof(int*), sizeof(int*) * nRows);
  /* not sure how many labels we're gonna need. max is pixels / 2 (by pigeonhole principle) */
  int* labels = (int*) calloc(sizeof(int), sizeof(int) * nRows * nCols / 2);
  int* nLabels = (int*) calloc(sizeof(int), sizeof(int) * nRows * nCols / 2);
  
  for(i = 0; i < nRows; i++)
    grid[i] = (int*) calloc(sizeof(int), sizeof(int) * nCols);
  
  convert(im, grid, labels, nRows, nCols);
  // the true label count
  int tlcount = 0;
  
  for(i = 1; labels[i]; i++)
    {
      if(!nLabels[labels[i]])
	nLabels[labels[i]] = tlcount++;
      //printf("%d,%d ", labels[i], nLabels[labels[i]]);
    }
  tlcount-=1;
  
  //  printf("Max: %d\n", tlcount);
  setColors(im, tlcount);
  for(i = 0; i < nRows; i++)
    for(j = 0; j < nCols; j++)
      setPixel(im, i, j, nLabels[labels[grid[i][j]]]);

  writeImage(im, argv[2]);
  
  free(labels);
  free(nLabels);
  for(i = 0; i < nRows; i++)
    free(grid[i]);
  free(grid);
}

void printArr(int **toPrint, int nRows, int nCols){
  int i,j;
  for(i=0; i<nRows; i++)
    {
      for(j=0; j<nCols; j++)
	printf("%d ", toPrint[i][j]);
      printf("%s","\n");
    }
}


void convert(Image* im, int **grid, int *labels, int nRows, int nCols){
  int lcount = 1;
  int i,j;
  
  for(i = 0; i < nRows; i++)
    for(j = 0; j < nCols; j++)
      if(getPixel(im, i, j)){ // replace with getPixel(im, i, j)
	    if(i > 0 && j > 0 && grid[i-1][j-1])
	      grid[i][j] = labels[grid[i-1][j-1]];
	    else if(j > 0 && grid[i][j-1] && (i==0 || grid[i-1][j] + grid[i-1][j-1] == 0))
	      grid[i][j] = labels[grid[i][j-1]];
	    else if(i > 0 && grid[i-1][j] && (j==0 || grid[i][j-1] + grid[i-1][j-1] == 0))
	      grid[i][j] = labels[grid[i-1][j]];
	    else if(i > 0 && j > 0 && grid[i-1][j] && grid[i][j-1])
	      if(grid[i-1][j] == grid[i][j-1])
	        grid[i][j] = labels[grid[i-1][j]];
	      else{
	        int minLabel = labels[grid[i][j-1]];
	        int maxLabel = labels[grid[i-1][j]];
	        int k;
	        if(labels[grid[i-1][j]] < labels[grid[i][j-1]])
	          {
		    int tmp = minLabel;
		    minLabel = maxLabel;
		    maxLabel = tmp;
	          }
	        for(k = 0; k < lcount; k++)
	          if(labels[k] == maxLabel)
		    labels[k] = minLabel;
	        grid[i][j] = labels[grid[i-1][j]];
	      }
	    else{
	      labels[lcount] = lcount;
	      grid[i][j] = lcount;
	      lcount++;
	    }
    }
}

/*
void testConvert(){
  int testSize = 3;
  int** test = (int**)malloc(sizeof(int*)*testSize);
  int** grid = (int**)malloc(sizeof(int*)*testSize);
  int i,j;
  for(i=0; i<testSize; i++){ 
    test[i] = (int*)malloc(sizeof(int)*testSize); 
    grid[i] = (int*)malloc(sizeof(int)*testSize); 
  }
  
  test[0][0] = 0;
  test[0][1] = 0;
  test[0][2] = 1;
  test[1][0] = 0;
  test[1][1] = 1;
  test[1][2] = 1;
  test[2][0] = 1;
  test[2][1] = 1;
  test[2][2] = 1;
  int* labels = (int*) calloc(sizeof(int), sizeof(int) * testSize * testSize / 2);

  convert(test, grid, labels, testSize, testSize);
  printf("%s", "print test:\n");
  printArr(test, testSize, testSize);
  printf("%s", "print grid:\n");
  printArr(grid, testSize, testSize);
}
*/
