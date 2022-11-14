#include <cmath>
#include <stdio.h>
#include <iostream>
#include "conn_comp_forloop.h"

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

using namespace std;


void conn_comp(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int width, int height) {
  int labelNo = 1;
  int index   = 0;
  int un_class[20];
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        if (input_image[index] == 0){
            output_image[index] = 0;
            continue;
        }
        int above = x+ (width-1)*y;
        int left = x-1 + width*y;
        int diag = x-1 + (width-1)*y;//12
        int A = y == 0? 0: input_image[above];
        int B = x== 0? 0: input_image[left];
        int C = ((x==0)||(y==0))?0: input_image[diag];//8
        if(A == 0 && B == 0 && C== 0){//6
          output_image[index] = labelNo;
          labelNo++;
          //assert (labelNo<20);
        }
        else if(A == 1 && B ==1 && C == 1){//12
          int labelA = output_image[above];
          int labelB = output_image[left];
          int labelC = output_image[diag];
          if((labelA <= labelB) && (labelA <= labelC)){
              output_image[index] = labelA;
              un_class[labelB] = labelA;
              un_class[labelC] = labelA;
          }
          else if((labelB <= labelC) && (labelB <= labelC)){
              output_image[index] = labelB;
              un_class[labelA] = labelB;
              un_class[labelC] = labelB;
          }
          else{
              output_image[index] = labelC;
              un_class[labelA] = labelC;
              un_class[labelB] = labelC;
          }
        }
        else if(A == 1 && B ==1){//5
          int labelA = output_image[above];
          int labelB = output_image[left];
          if(labelA <= labelB){
              output_image[index] = labelA;
              un_class[labelB] = labelA;
          }
          else{
              output_image[index] = labelB;
              un_class[labelA] = labelB;
          }
        }
        else if(A == 1 && C ==1){//5
          int labelA = output_image[above];
          int labelC = output_image[diag];
          if(labelA <= labelC){
              output_image[index] = labelA;
              un_class[labelC] = labelA;
          }
          else{
              output_image[index] = labelC;
              un_class[labelA] = labelC;
          }
        }
        else if(B == 1 && C ==1){//5
          int labelB = output_image[left];
          int labelC = output_image[diag];
          if(labelB <= labelC){
              output_image[index] = labelB;
              un_class[labelC] = labelB;
          }
          else{
              output_image[index] = labelC;
              un_class[labelB] = labelC;
          }
        }
        else if(A == 1){
          int labelA = output_image[above];
          output_image[index] = labelA;
        }
        else if(B == 1){
          int labelB = output_image[left];
          output_image[index] = labelB;
        }
        else if(C == 1){//3
          int labelC = output_image[diag];
          output_image[index] = labelC;
        }
  
    }
  }
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        int label = output_image[index];
        if(label!=0){
            int correct_label = un_class[label];
            if(correct_label!=label){
                output_image[index] = correct_label;
            }
        }
    }
  }
}
int main(){
  int test_array[9];
  test_array[0] = 1;
  test_array[1] = 1;
  test_array[2] = 1;
  test_array[3] = 0;
  test_array[4] = 0;
  test_array[5] = 1;
  test_array[6] = 1;
  test_array[7] = 0;
  test_array[8] = 0;
  int test_output[9];
  conn_comp(test_array[NUM_PIXELS], test_output[NUM_PIXELS], 3, 3);
  for (int i = 0; i<3; i++){
    for (int j = 0; j<3; j++){
      int index = j+i*3
      cout<<test_output[index];
    }
    cout<<endl;
  }
}




