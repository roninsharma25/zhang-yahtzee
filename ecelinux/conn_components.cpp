#include <cmath>
#include <stdio.h>
#include "conn_components.h"
#include <cassert> 

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

pixel labelNo = 1;

pixel labelA;
pixel labelB;
pixel labelC;
pixel labelD;

pixel output;

int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256]) {
  if ( (A == 0) && ((labelA <= labelB) || B == 1) && ((labelA <= labelC) || C == 1) && ((labelA <= labelD) || D == 1 )) {
    output = labelA;
    if (B == 0) {
      un_class[labelB] = labelA;
    }
    if (C == 0) {
      un_class[labelC] = labelA;
    }
    if (D == 0) {
      un_class[labelD] = labelA;
    }
    return 0;
  }
  return 1;
}


pixel conn_comp_1st_pass( buf_bit in_buffer, buf_8 out_buffer, pixel un_class[256], int width, int height, int x, int y) {
  
  bit in = in_buffer[0];
  if(in==1){
    return 255;
  }
  bit A = y == 0? 1: in_buffer[width];
  bit B = x== 0? 1: in_buffer[1];
  bit C = ((x==0)||(y==0))?1: in_buffer[width+1];//8
  bit D = ((x==width-1)||(y==0))?1: in_buffer[width-1];
  if(A && B && C && D){
    output= labelNo;
    un_class[labelNo] = labelNo;

    labelNo++;
    assert (labelNo<256);
  } else {

    labelA = out_buffer(width*8+7,width*8);
    labelB = out_buffer(15,8);
    labelC = out_buffer((width+1)*8+7,(width+1)*8);
    labelD = out_buffer((width-1)*8+7,(width-1)*8);

    int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class);
    int option_2, option_3;
    if (option_1) {
      option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class);
    }
    if (option_1 && option_2) {
      option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class);
    }
    if (option_1 && option_2 && option_3) {
      un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class);
    }

  }

  return output;
}

void conn_comp_2nd_pass(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int* un_class, int width, int height ){
  int size_class[256];
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      int index = x + width*y;
      int label = input_image[index];
      if(label!=0){
        int correct_label = un_class[label];
        size_class[correct_label] += 1;
        if(correct_label!=label){
            output_image[index] = correct_label;
        }
      }
    }
  }
}


