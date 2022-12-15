// Reference: https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

#include <stdio.h>
#include "conn_components.h"
#include <cassert> 

//pixel labelNoB = 1;
//pixel labelNoW = 1;
//pixel outputB;
//pixel outputW;


int un_class_method(bit A, bit B, bit C, bit D, pixel labelA, pixel labelB, pixel labelC, pixel labelD, pixel un_class[256], bit type, pixel *output) {

  if ( (A == type) && ((labelA <= labelB) || B == !type) && ((labelA <= labelC) || C == !type) && ((labelA <= labelD) || D == !type )) {
    if (type) {
      *output = labelA;
    } else {
      *output = labelA;
    }
    //outputB = labelA;
    if (B == type) un_class[labelB] = labelA;
    if (C == type) un_class[labelC] = labelA;
    if (D == type) un_class[labelD] = labelA;

    return 0;
  }

  return 1;

}

pixel conn_comp_1st_pass_black(buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel label[256], buf_8 out_bufferW, pixel *labelNoB) {
  pixel outputB;
  bit in = in_buffer[0];
  if (in == 1) return 0;
  //if (x == 0 && y == 0 ) *labelNoB = 1;
  
  bit A = y == 0 ? (bit) 1 : in_buffer[width];
  bit B = x == 0 ? (bit) 1 : in_buffer[1];
  bit C = ( (x == 0) || (y == 0) ) ? (bit) 1: in_buffer[width+1];
  bit D = ( (x == width-1) || (y == 0) ) ? (bit) 1 : in_buffer[width-1];

  if (A && B && C && D) {
  
    outputB = *labelNoB;
    un_class[*labelNoB] = *labelNoB;
    label[*labelNoB] = out_bufferW(15,8);
    *labelNoB = *labelNoB + 1;
    //assert (labelNoB<256);

  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)( (width+1)*8+7, (width+1)*8 );
    pixel labelD = (*out_buffer)( (width-1)*8+7, (width-1)*8 );

    int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class, 0, &outputB);
    int option_2, option_3;
    if (option_1) option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class, 0, &outputB);
    if (option_1 && option_2) option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class, 0, &outputB);
    if (option_1 && option_2 && option_3) un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class, 0, &outputB);

  }

  return outputB;
}

pixel conn_comp_1st_pass_white( buf_bit in_buffer, buf_8 *out_buffer, pixel un_class[256], int width, int height, int x, int y, pixel *labelNoW) {
  pixel outputW;
  bit in = in_buffer[0];
  if (in == 0) return 0;
  //if (x == 0 && y == 0) *labelNoW = 1;
  
  bit A = y == 0 ? (bit) 0 : in_buffer[width];
  bit B = x == 0 ? (bit) 0 : in_buffer[1];
  bit C = ( (x == 0) || (y == 0) ) ? (bit) 0 : in_buffer[width+1];
  bit D = ( (x == width-1) || (y == 0) ) ? (bit) 0 : in_buffer[width-1];

  if ( A == 0 && B == 0 && C == 0 && D == 0 ) {
  
    outputW= *labelNoW;
    un_class[*labelNoW] = *labelNoW;
    *labelNoW = *labelNoW + 1;
    //assert (labelNoW<256);
  
  } else {

    pixel labelA = (*out_buffer)(width*8+7,width*8);
    pixel labelB = (*out_buffer)(15,8);
    pixel labelC = (*out_buffer)( (width+1)*8+7, (width+1)*8 );
    pixel labelD = (*out_buffer)( (width-1)*8+7, (width-1)*8 );

    int option_1 = un_class_method(A, B, C, D, labelA, labelB, labelC, labelD, un_class, 1, &outputW);
    int option_2, option_3;
    if (option_1) option_2 = un_class_method(B, A, C, D, labelB, labelA, labelC, labelD, un_class, 1, &outputW );
    if (option_1 && option_2) option_3 = un_class_method(C, A, B, D, labelC, labelA, labelB, labelD, un_class, 1, &outputW);
    if (option_1 && option_2 && option_3) un_class_method(D, A, B, C, labelD, labelA, labelB, labelC, un_class, 1, &outputW);
  }

  return outputW;
}

void mergeB( buf_8 maB, buf_8 mbB,buf_bit mid_b, buf_bit mid_a, int size[256], int size2[256] ){
  pixel prev = 0;
  for(int i = 0; i<410;i++){
    if(mid_b == 0 && mid_a == 0){//black same class, not white/0, //using sizing could be problamtic, another solution would be to act like white
      //add size together
      //set size of other class to 0
      int index = maB(i*8+7, i*8);
      if(index != prev){
        prev = index;
        int index2 =  mbB(i*8+7, i*8);
        size2[index2] += size[index];
        size[index] = 0;
      }
    }
    //go to next black class, have a marker that holds previous class
  }
}

void mergeW( buf_8 maW, buf_8 mbW, buf_bit mid_b, buf_bit mid_a, pixel un_classW[256], pixel un_classW2[256]){
  //if((mid_a == 1) && (mid_a[0]==mid_b[0])){
  //  //dont do two indexing if when looking at balck dice, always use smallest index value
  //  int index = un_classW[(int)maW(7,0)]; 
  //  un_classW[index] = un_classW[(int)maW(7,0)]
  //}
  bool branch = 1;
  for(int i = 0; i<410;i++){
    if(mid_a[i] && mid_b[i]&& branch){
      int index = un_classW[(int)maW(i*8+7,i*8)];
      un_classW[index] = un_classW2[(int)mbW(i*8+7,i*8)];
      branch = 0;
    }
    else if(branch == 0 && mid_a[i]==0){
      branch = 1;
    }
  }
  //for loop
  //if same white class
  //change un_clasW to have value of un_classW2
  //go to next white class
}
