// Reference: https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

#include <stdio.h>
#include "conn_components_base.h"
#include <cassert> 

pixel labelNoB = 1;
pixel labelNoW = 1;
pixel outputB;
pixel outputW;

int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256], bit type) {

  if ( (A == type) && ((labelA <= labelB) || B == !type) && ((labelA <= labelC) || C == !type) && ((labelA <= labelD) || D == !type )) {
    if (type) {
      outputW = labelA;
    } else {
      outputB = labelA;
    }
    //outputB = labelA;
    if (B == type) un_class[labelB] = labelA;
    if (C == type) un_class[labelC] = labelA;
    if (D == type) un_class[labelD] = labelA;

    return 0;
  }

  return 1;

}

pixel conn_comp_1st_pass_black(base_buf_bit in_buffer, base_buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], base_buf_8 out_bufferW) {
  bit in = in_buffer[0];
  if (in == 1) return 0;
  if (x == 0 && y == 0 ) labelNoB = 1;
  
  bit A = y == 0 ? (bit) 1 : in_buffer[width];
  bit B = x == 0 ? (bit) 1 : in_buffer[1];
  bit C = ( (x == 0) || (y == 0) ) ? (bit) 1: in_buffer[width+1];
  bit D = ( (x == width-1) || (y == 0) ) ? (bit) 1 : in_buffer[width-1];

  if (A && B && C && D) {
  
    outputB = labelNoB;
    un_class[labelNoB] = labelNoB;
    label[labelNoB] = out_bufferW(15,8);
    labelNoB++;
    assert (labelNoB<256);

  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)( (width+1)*8+7, (width+1)*8 );
    pixel labelD = (*out_buffer)( (width-1)*8+7, (width-1)*8 );

    int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class, 0);
    int option_2, option_3;
    if (option_1) option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class, 0);
    if (option_1 && option_2) option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class, 0);
    if (option_1 && option_2 && option_3) un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class, 0);
  }

  return outputB;
}

pixel conn_comp_1st_pass_white( base_buf_bit in_buffer, base_buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y) {
  bit in = in_buffer[0];
  if (in == 0) return 0;
  if (x == 0 && y == 0) labelNoW = 1;
  
  bit A = y == 0 ? (bit) 0 : in_buffer[width];
  bit B = x == 0 ? (bit) 0 : in_buffer[1];
  bit C = ( (x == 0) || (y == 0) ) ? (bit) 0 : in_buffer[width+1];
  bit D = ( (x == width-1) || (y == 0) ) ? (bit) 0 : in_buffer[width-1];

  if ( A == 0 && B == 0 && C == 0 && D == 0 ) {
  
    outputW= labelNoW;
    un_class[labelNoW] = labelNoW;
    labelNoW++;
    assert (labelNoW<256);
  
  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)( (width+1)*8+7, (width+1)*8 );
    pixel labelD = (*out_buffer)( (width-1)*8+7, (width-1)*8 );

    int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class, 1);
    int option_2, option_3;
    if (option_1) option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class, 1);
    if (option_1 && option_2) option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class, 1);
    if (option_1 && option_2 && option_3) un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class, 1);
  }

  return outputW;
}
