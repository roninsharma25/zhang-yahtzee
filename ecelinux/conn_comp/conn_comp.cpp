#include <cmath>
#include <stdio.h>
#include <iostream>
#include "conn_comp.h"

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

using namespace std;

void conn_comp(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int width, int height) {
  int labelNo = 0;
  int index   = -1;
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      index++;
      if (input_image[index] == 0) continue;   /* This pixel is not part of a component */
      if (output_image[index] != 0) continue;   /* This pixel has already been labelled  */
      /* New component found */
      labelNo++;
      LabelComponent(width, height, input_image, output_image, labelNo, x, y);
    }
  }
}

void LabelComponent(int width, int height, 
int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int labelNo, int x, int y)
{
  int index = x + width*y;
  if (input_image[index] == 0) return;   /* This pixel is not part of a component */
  if (output_image[index] != 0) return;   /* This pixel has already been labelled  */
  output_image[index] = labelNo;

  /* Now label the 4 neighbours: */
  if (x > 0)        LabelComponent(width, height, input_image, output_image, labelNo, x-1, y);   /* left  pixel */
  if (x < width-1)  LabelComponent(width, height, input_image, output_image, labelNo, x+1, y);   /* right pixel */
  if (y > 0)        LabelComponent(width, height, input_image, output_image, labelNo, x, y-1);   /* upper pixel */
  if (y < height-1) LabelComponent(width, height, input_image, output_image, labelNo, x, y+1);   /* lower pixel */
}