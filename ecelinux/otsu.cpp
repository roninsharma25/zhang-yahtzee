#include<cmath>
#include <stdio.h>
#include <iostream>
#include "otsu.h"

// https://www.ipol.im/pub/art/2016/158/article_lr.pdf

using namespace std;

// LARGEST PIXEL VALUE CURRENTLY SET TO 255
int threshold;
float weighted_sum, intermediate_sum, q1, q2, mu1, mu2, variance, max_variance;

// Class 1: pixels below current threshold
// Class 2: pixels above current threshold

// q1: probability function for class 1
// q2: probability function for class 2
// mu1: mean of class 1
// mu2: mean of class 2
// variance: inter-class variance

// int histogram[256];
//int[] otsu_output_image;

void update_histogram(bit32_t pixel_value, int histogram[]) {
    for(int i = 0; i < 4; i++){
        ap_uint<8> chunk = pixel_value((i << 3) + 7, (i << 3));
        histogram[chunk] += 1;
    }
}


pixel otsu(int histogram[]) {
    // printf("i am in otsu\n");
    // Calculate the weighted sum of all bins
    for (int i = 0; i < 256; i++) {
        weighted_sum += i * histogram[i];
    }

    q1 = 0.0f;
    q2 = 0.0f;
    // Calculate variances
    for (int i = 0; i < 256; i++) {
        // printf("histogram[%d] = %d\n", i, histogram[i]);
        q1 += histogram[i];
        if (q1 == 0) continue;
        q2 = NUM_PIXELS - q1;
        if (q2 == 0) continue;
        // printf("q1:%f, q2:%f", q1, q2);

        intermediate_sum += i * histogram[i];
        mu1 = intermediate_sum / q1;
        mu2 = (weighted_sum - intermediate_sum) / q2;
        // printf("intsum:%f, mu1:%f, mu2:%f\n", intermediate_sum, mu1, mu2);

        // Minimize intra-class variance = maximize inter-class variance
        variance = q1 * q2 * (mu1 - mu2)*(mu1 - mu2) ;
        // printf("var:%f max:%f\n", variance, max_variance);  
        if (variance >= max_variance) {
            threshold = i;
            // printf("i am in variance");
            max_variance = variance;
        }
    }

    return threshold;
}

bit threshold_image(pixel input, pixel threshold_input){
    // Perform thresholding
    bit output = (input.to_int() >= threshold_input.to_int()) ? 1 : 0;
    //printf("input: %d, threshold: %d, output: %d\n", input.to_int(), threshold_input.to_int(), output.to_int());
    return output;
}

int sum (int a, int b) { return a + b; }
