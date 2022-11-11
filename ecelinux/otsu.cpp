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

    // Calculate the weighted sum of all bins
    for (int i = 0; i < 256; i++) {
        weighted_sum += i * histogram[i];
    }

    // Calculate variances
    for (int i = 0; i < 256; i++) {
        q1 += histogram[i];
        if (q1 == 0) continue;
        q2 = NUM_PIXELS - q1;
        if (q2 == 0) continue;

        intermediate_sum += i * histogram[i];
        mu1 = intermediate_sum / q1;
        mu2 = (weighted_sum - intermediate_sum) / q2;

        // Minimize intra-class variance = maximize inter-class variance
        variance = q1 * q2 * pow(mu1 - mu2, 2);

        if (variance >= max_variance) {
            threshold = i;
            max_variance = variance;
        }
    }

    return threshold;
}

// void threshold_image(hls::stream<pixel> &input, hls::stream<bit> &output,
//  pixel threshold){
//     // Perform thresholding
//     for (int i = 0; i < NUM_PIXELS; i++) {
//         if (input_image[i] > threshold) {
//             output_image[i] = 1;
//         } else {
//             output_image[i] = 0;
//         }
//     }
// }