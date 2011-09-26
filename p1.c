#include"vision_utilities.h"
#include<stdio.h>
#include<stdlib.h>
Image* im;
int thresh;


int main(int nargin, char** argv){
  if(nargin != 4){
    printf("Usage: p1 <greylevel img> <threshold> <output file>");
    return -1;
  }
  
  im = (Image*) malloc(sizeof(Image));
  thresh = atoi(argv[2]);
  readImage(im, argv[1]);
  int i, j;
  setColors(im, 1);
  for(i = 0; i < getNRows(im); i++){
    for(j = 0; j < getNCols(im); j++){
      setPixel(im, i, j, getPixel(im, i, j) > thresh);
    }
  }
  writeImage(im, argv[3]);
}
