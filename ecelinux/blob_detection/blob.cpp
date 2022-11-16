#define NUM_PIXELS 100 // UPDATE



int blob_counter(pixel input_array[]) {
    for (int i = NUM_PIXELS - 1; i > 0; i--) {
        if (input_array[i].to_int() != 0) {
            return input_array[i].to_int() - 1;
        }
    }

    return 0;
}