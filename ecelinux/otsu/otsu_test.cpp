#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "otsu.h"

using namespace std;

int main() {

    ifstream inputfile("test.txt");
    int input_image_array[ROW][COL]; // NEED TO CONVERT THE STRING TO AN INT
    int output_image_array[ROW][COL];

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
          inputfile >> input_image_array[r][c];
        }
    }

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
          std::cout << input_image_array[r][c] << "\n";
        }
    }

    int combined_input_array[ROW * COL];
    int combined_output_array[ROW * COL];
    int current_index = 0;
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
          combined_input_array[current_index] = input_image_array[r][c];
          current_index++;
        }
    }

    int threshold = otsu(combined_input_array, combined_output_array);

    current_index = 0;
    // Write the output to the output image
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
          output_image_array[r][c] = combined_output_array[current_index];
          current_index++;
        }
    }

    std::cout << "Output Array" << "\n";
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
          std::cout << output_image_array[r][c] << "\n";
        }
    }

    std::cout << "Threshold" << "\n";
    std::cout << threshold << "\n";

    std::cout << "Done" << std::endl;

    return 0;
}

