#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "otsu.h"

using namespace std;

int main() {
    int arr1[5] = {0, 1, 2, 3, 4};
    int arr2[5] = {0, 0, 0, 0, 0};
    otsu(arr1, arr2);

    for (int i = 0; i < 5; i++) {
        std::cout << arr2[i] << "\n";
    }

    std::cout << "Done" << std::endl;

    return 0;
}

