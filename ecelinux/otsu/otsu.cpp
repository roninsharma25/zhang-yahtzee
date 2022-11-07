#include<cmath>
#include "otsu.h"

// https://www.ipol.im/pub/art/2016/158/article_lr.pdf


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

void otsu(int input_image[NUM_PIXELS], int output_image[NUM_PIXELS]) {
    // Create the empty histogram - bins for each pixel value
    int histogram[255];

    for (int i = 0; i < 255; i++) {
        histogram[i] = 0;
    }

    // Update the histogram based on the input image
    for (int i = 0; i < NUM_PIXELS; i++) {
        histogram[input_image[i]] += 1;
    }

    // Calculate the weighted sum of all bins
    for (int i = 0; i < 255; i++) {
        weighted_sum += i * histogram[i];
    }

    // Calculate variances
    for (int i = 0; i < 255; i++) {
        q1 += histogram[i];
        if (q1 == 0) continue;
        q2 = 255 - q1;

        intermediate_sum += i * histogram[i];
        mu1 = intermediate_sum / q1;
        mu2 = (weighted_sum - intermediate_sum) / q2;

        // Minimize intra-class variance = maximize inter-class variance
        variance = q1 * q2 * pow(mu1 - mu2, 2);

        if (variance > max_variance) {
            threshold = i;
            max_variance = variance;
        }
    }

    // Perform thresholding
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (input_image[i] > threshold) {
            output_image[i] = 1;
        } else {
            output_image[i] = 0;
        }
    }
}