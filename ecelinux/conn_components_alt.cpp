// Reference: https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

#include <stdio.h>
#include "conn_components.h"
#include <cassert> 

pixel labelNoB = 1;
pixel labelNoW = 1;
pixel outputB;
pixel outputW;

int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256], bit type) {
  
  if ( (A == type) && ((labelA <= labelB) || B == !type) && ((labelA <= labelC) || C == !type) && ((labelA <= labelD) || D == !type )) {
    //printf("s");
    if (type) {
      outputW = labelA;
    } else {
      outputB = labelA;
    }
    //outputB = labelA;

    if (B == type && un_class[labelB] < labelA && un_class[labelB] != 0) labelA = un_class[labelB]; 
    if (C == type && un_class[labelC] < labelA && un_class[labelC] != 0) labelA = un_class[labelC]; 
    if (D == type && un_class[labelD] < labelA && un_class[labelD] != 0) labelA = un_class[labelD]; 

    if (B == type && (un_class[labelB] > labelA || un_class[labelB] == 0)) un_class[labelB] = labelA;
    if (C == type && (un_class[labelC] > labelA || un_class[labelC] == 0)) un_class[labelC] = labelA;
    if (D == type && (un_class[labelD] > labelA || un_class[labelD] == 0)) un_class[labelD] = labelA;

    //std::cout << "bit A " << A << " label A " << labelA << std::endl;
    //std::cout << "bit B " << B << " label B " << labelB << std::endl;
    //std::cout << "bit C " << C << " label C " << labelC << std::endl;
    //std::cout << "bit D " << D << " label D " << labelD << std::endl;
    //std::cout << "unclass B " << un_class[labelB] << std::endl; 
    //std::cout << "unclass C " << un_class[labelC] << std::endl; 
    //std::cout << "unclass D " << un_class[labelD] << std::endl; 
    return 0;
  }

  return 1;

}

void link_pixel (buf_bit in_buffer, buf_8 *clas_buffer, pixel un_class[256], int width, int pixel_class, bit type) {
  bit A = in_buffer[width]; 
  bit B = type; 
  bit C = in_buffer[width+1]; 
  bit D = in_buffer[width-1]; 

  if (A != type && C != type && D != type) return; 

  pixel labelA = (*clas_buffer)(width*8+7,width*8); 
  pixel labelB = pixel_class; 
  pixel labelC = (*clas_buffer)( (width+1)*8+7, (width+1)*8 );
  pixel labelD = (*clas_buffer)( (width-1)*8+7, (width-1)*8 );


  //std::cout << "bit A " << A << " label A " << labelA << std::endl;
  //std::cout << "bit B " << B << " label B " << labelB << std::endl;
  //std::cout << "bit C " << C << " label C " << labelC << std::endl;
  //std::cout << "bit D " << D << " label D " << labelD << std::endl;

  int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class, type);
  int option_2 = 0; 
  int option_3 = 0;
  if (option_1) option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class, type);
  if (option_1 && option_2) option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class, type);
  if (option_1 && option_2 && option_3) un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class, type);
  //printf("option 1: %d , option 2: %d, option 3: %d\n", option_1, option_2, option_3);
  return;
}

pixel conn_comp_1st_pass_black(buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], buf_8 out_bufferW) {
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

pixel conn_comp_1st_pass_white( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y) {
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
