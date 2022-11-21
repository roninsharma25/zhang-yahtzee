#include <cmath>
#include <stdio.h>
#include "conn_components.h"
#include <cassert> 

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

pixel labelNoB = 1;
pixel labelNoW = 1;

pixel outputB;
pixel outputW;

int un_class_method_black(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256]) {
  if ( (A == 0) && ((labelA <= labelB) || B == 1) && ((labelA <= labelC) || C == 1) && ((labelA <= labelD) || D == 1 )) {
    outputB = labelA;
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

int un_class_method_white(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256]) {
  if ( (A == 1) && ((labelA <= labelB) || B == 0) && ((labelA <= labelC) || C == 0) && ((labelA <= labelD) || D == 0 )) {
    outputW = labelA;
    if (B == 1) {
      un_class[labelB] = labelA;
    }
    if (C == 1) {
      un_class[labelC] = labelA;
    }
    if (D == 1) {
      un_class[labelD] = labelA;
    }
    return 0;
  }
  return 1;
}


pixel conn_comp_1st_pass_black( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], buf_8 out_bufferW) {
  bit in = in_buffer[0];
  if(in==1){
    return 0;
  }
  bit A = y == 0? (bit)1: in_buffer[width];
  bit B = x== 0? (bit)1: in_buffer[1];
  bit C = ((x==0)||(y==0))?(bit)1: in_buffer[width];// REMOVED + 1
  bit D = ((x==width-1)||(y==0))?(bit)1: in_buffer[width-1];

  if(A && B && C && D){
    outputB= labelNoB;
    un_class[labelNoB] = labelNoB;
    label[labelNoB] = out_bufferW(15,8);
    labelNoB++;
    assert (labelNoB<256);
  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)((width)*8+7,(width)*8); // REMOVED + 1
    pixel labelD = (*out_buffer)((width-1)*8+7,(width-1)*8);

    int option_1 = un_class_method_black(A, B, C, D, labelA, labelB, labelC, labelD, un_class);
    int option_2, option_3;
    if (option_1) {
      option_2 = un_class_method_black(B, A, C, D, labelB, labelA, labelC, labelD, un_class);
    }
    if (option_1 && option_2) {
      option_3 = un_class_method_black(C, A, B, D, labelC, labelA, labelB, labelD, un_class);
    }
    if (option_1 && option_2 && option_3) {
      un_class_method_black(D, A, B, C, labelD, labelA, labelB, labelC, un_class);
    }

  }

  return outputB;
}

pixel conn_comp_1st_pass_white( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y) {
  bit in = in_buffer[0];
  if(in==0){
    return 0;
  }
  bit A = y == 0? (bit)0: in_buffer[width];
  bit B = x== 0? (bit)0: in_buffer[1];
  bit C = ((x==0)||(y==0))?(bit)0: in_buffer[width]; // REMOVED + 1
  bit D = ((x==width-1)||(y==0))?(bit)0: in_buffer[width-1];

  if(A==0 && B==0 && C==0 && D==0){
    outputW= labelNoW;
    un_class[labelNoW] = labelNoW;

    labelNoW++;
    assert (labelNoW<256);
  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)((width)*8+7,(width)*8); // REMOVED + 1
    pixel labelD = (*out_buffer)((width-1)*8+7,(width-1)*8);

    int option_1 = un_class_method_white(A, B, C, D, labelA, labelB, labelC, labelD, un_class);
    int option_2, option_3;
    if (option_1) {
      option_2 = un_class_method_white(B, A, C, D, labelB, labelA, labelC, labelD, un_class);
    }
    if (option_1 && option_2) {
      option_3 = un_class_method_white(C, A, B, D, labelC, labelA, labelB, labelD, un_class);
    }
    if (option_1 && option_2 && option_3) {
      un_class_method_white(D, A, B, C, labelD, labelA, labelB, labelC, un_class);
    }

  }

  return outputW;
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

