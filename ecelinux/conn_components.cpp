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

int un_class_method(bit A, bit B, pixel labelA, pixel labelB, pixel un_class[256]) {
  if ( (A == 1) && ( (labelA <= labelB) || B == 0 ) ) {
    output = labelA;
    printf("BEFORE SAME LABEL \n");
    if (B == 1) {
      un_class[labelB] = labelA;
      printf("SAME LABEL \n");
    }
    return 0;
  }
  return 1;
}


pixel conn_comp_1st_pass( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y) {
  
  bit in = in_buffer[0];
  if (in == 0) {
    return 255;
  }

  bit A = (y == 0) ? 0 : in_buffer[width];
  bit B = (x == 0) ? 0 : in_buffer[1];

  // for (int z = 0; z < 3)

  if (A == 0 && B == 0) {

    printf("A 0 B 0 \n");

    output = labelNo;
    un_class[labelNo] = labelNo;
    labelNo++;
    assert (labelNo<256);

  } else {

    printf("OTHER \n");
    printf("A: %d \n", A.to_int());

    labelA = (*out_buffer)(width*8+7,width*8);
    labelB = (*out_buffer)(15,8);
    int option_1 = un_class_method(A, B, labelA, labelB, un_class);
    if (option_1) un_class_method(B, A, labelB, labelA, un_class);

  }

  if (A == 1) { 
    for (int i = 1; i < width; i++) {
      if (in_buffer[i] != 0 && labelA <= (*out_buffer)((i*8) + 7, (i*8))){
        printf("IN CC NUM 1: %d \n", (*out_buffer)(15,8).to_int());
        (*out_buffer)((i*8) + 7, (i*8)) = labelA;
        printf("ASFASFASF: %d - %d - %d\n", labelA.to_int(), (*out_buffer)(i*8 + 7, i*8).to_int(), i);
        
      } 
      else return output;
    }
  }

  return output;
}

void conn_comp_2nd_pass(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int* un_class, int width, int height ){
  int size_class[256];
  
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int index = x + width*y;
      int label = input_image[index];
      if (label != 0) {
        int correct_label = un_class[label];
        size_class[correct_label] += 1;
        if (correct_label != label ){
            output_image[index] = correct_label;
        }
      }
    }
  }
}

