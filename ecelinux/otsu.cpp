// Reference article: https://www.ipol.im/pub/art/2016/158/article_lr.pdf

// Class 1: pixels below current threshold
// Class 2: pixels above current threshold

// q1: probability function for class 1
// q2: probability function for class 2
// mu1: mean of class 1
// mu2: mean of class 2
// variance: inter-class variance

#include <stdio.h>
#include <iostream>
#include "otsu.h"

using namespace std;

int threshold;
float weighted_sum = 0;
float intermediate_sum = 0;
float q1 = 0;
float q2 = 0;
float mu1 = 0;
float mu2 = 0;
float variance = 0;
float max_variance = 0;

void update_histogram(bit32_t pixel_value, int histogram[]) {
    for(int i = 0; i < 4; i++){
        ap_uint<8> chunk = pixel_value((i << 3) + 7, (i << 3));
        histogram[chunk] += 1;
    }
}

pixel otsu(int histogram[]) {
    // Calculate the weighted sum of all bins
    weighted_sum = 0;
    for (int i = 0; i < 256; i++) {
        weighted_sum += i * histogram[i];
    }

    q1 = 0.0f;
    q2 = 0.0f;

    // Calculate variances
    intermediate_sum = 0;
    mu1 = 0;
    mu2 = 0;
    max_variance = 0;
    for (int i = 0; i < 256; i++) {
        q1 += histogram[i];
        if (q1 == 0) continue;
        q2 = NUM_PIXELS - q1;
        if (q2 == 0) continue;

        intermediate_sum += i * histogram[i];
        mu1 = intermediate_sum / q1;
        mu2 = (weighted_sum - intermediate_sum) / q2;

        // Minimize intra-class variance = maximize inter-class variance
        variance = q1 * q2 * (mu1 - mu2)*(mu1 - mu2) ;
        if (variance >= max_variance) {
            threshold = i;
            max_variance = variance;
        }
    }

    return threshold;
}

bit threshold_image(pixel input, pixel threshold_input){
    // Perform thresholding
    bit output = (input.to_int() >= threshold_input.to_int()) ? 1 : 0;

    return output;
}
