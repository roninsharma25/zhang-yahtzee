#include <cmath>
#include <stdio.h>
#include <iostream>
#include "conn_comp.h"

// https://www.codeproject.com/Articles/825200/An-Implementation-Of-The-Connected-Component-Label

using namespace std;

void conn_comp(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS], int width, int height) {
  int labelNo = 1;
  int index   = 0;
  int union[20];
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
        index = x + width*y;
        if (input_image[index] == 0){
            outout_image[index] = 0;
            continue;
        }
        int above = x+ (width-1)*y;
        int left = x-1 + width*y;
        int diag = x-1 + (width-1)*y;
        int A = y == 0? 0, input_image[above];
        int B = x== 0? 0, input_image[left];
        int C = ((x==0)||(y==0))?0, input_image[diag];
        if(A == 0 && B == 0 && C== 0){
          output_image[index] = labelNo;
          labelNo++;
          assert (labelNo<20);
        }
        else if(A == 1 && B ==1 && C == 1){
          labelA = output_image[above];
          labelB = output_image[left];
          labelC = output_image[diag];
          if((labelA <= labelB) && (labelA <= labelC)){
              output_image[index] = labelA;
              union[labelB] = labelA;
              union[labelC] = labelA;
          }
          else if((labelB <= labelC) && (labelB <= labelC)){
              output_image[index] = labelB;
              union[labelA] = labelB;
              union[labelC] = labelB;
          }
          else{
              output_image[index] = labelC;
              union[labelA] = labelC;
              union[labelB] = labelC;
          }
        }
        else if(A == 1 && B ==1){
          labelA = output_image[above];
          labelB = output_image[left];
          if(labelA <= labelB){
              output_image[index] = labelA;
              union[labelB] = labelA;
          }
          else{
              output_image[index] = labelB;
              union[labelA] = labelB;
          }
        }
        else if(A == 1 && C ==1){
          labelA = output_image[above];
          labelC = output_image[diag];
          if(labelA <= labelC){
              output_image[index] = labelA;
              union[labelC] = labelA;
          }
          else{
              output_image[index] = labelC;
              union[labelA] = labelC;
          }
        }
        else if(B == 1 && C ==1){
          labelB = output_image[left];
          labelC = output_image[diag];
          if(labelB <= labelC){
              output_image[index] = labelB
              union[labelC] = labelB;
          }
          else{
              output_image[index] = labelC;
              union[labelB] = labelC;
          }
        }
        else if(A == 1){
          output[image] = labelA;
        }
        else if(B == 1){
          output[image] = labelB;
        }
        else if(C == 1){
            output[image] = labelC;
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
            int correct_label = union[label];
            if(correct_label!=label){
                output_image[index] = correct_label;
            }
        }
    }
  }
}

