#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>
#include<assert.h>

#include<iostream>
#include<fstream>

#include "typedefs.h"
#include "timer.h"

//------------------------------------------------------------------------
// Helper function for hex to int conversion
//------------------------------------------------------------------------
int64_t hexstring_to_int64 (std::string h) {
  int64_t x = 0;
  for (int i = 0; i < h.length(); ++i) {
    char c = h[i];
    int k = (c > '9') ? toupper(c)-'A'+10 : c - '0';
    x = x*16 + k;
  }
  return x;
}

//--------------------------------------
// main function
//--------------------------------------
int main(int argc, char** argv)
{
  // Open channels to the FPGA board.
  // These channels appear as files to the Linux OS
  int fdr = open("/dev/xillybus_read_32", O_RDONLY);
  int fdw = open("/dev/xillybus_write_32", O_WRONLY);

  // Check that the channels are correctly opened
  if ((fdr < 0) || (fdw < 0)) {
    fprintf (stderr, "Failed to open Xillybus device channels\n");
    exit(-1);
  }
  
  // Read input file for the testing set
  std::string line;
  std::ifstream myfile ("data/testing_set.dat");
  
  // Number of test instances
  const int N = 180;
  
  // Arrays to store test data and expected results
  digit inputs[N];
  int   expecteds[N];

  // Timer
  Timer timer("digitrec FPGA");
  
  int nbytes;
  int error = 0;
  int num_test_insts = 0;
  bit32_t interpreted_digit;


  if ( myfile.is_open() ) {
    
    //--------------------------------------------------------------------
    // Read data from the input file into two arrays
    //--------------------------------------------------------------------
    for (int i = 0; i < N; ++i) {
      assert( std::getline( myfile, line) );
      // Read handwritten digit input
      std::string hex_digit = line.substr(2, line.find(",")-2);
      digit input_digit = hexstring_to_int64 (hex_digit);
      // Read expected digit
      int input_value =
          strtoul(line.substr(line.find(",") + 1,
                              line.length()).c_str(), NULL, 10);
   
      // Store the digits into arrays
      inputs[i] = input_digit;
      expecteds[i] = input_value;
    }

    timer.start();

    //--------------------------------------------------------------------
    // Add your code here to communicate with the hardware module
    //--------------------------------------------------------------------
    digit test_digit;
    for (int i = 0; i < sizeof(inputs), i++){
      test_digit = inputs[i];

      bit64_t test_digit_i;
      test_digit_i(test_digit.length()-1,0) = test_digit(test_digit.length()-1,0);
      int64_t input = test_digit_i;

      nbytes = write (fdw, (void*)&test_digit, sizeof(test_digit);
      assert(nbytes == sizeof(test_digit));

      int32_t guess_out;
      nbytes = read (fdr, (void*)&guess_out, sizeof(guess_out));
      assert (nbytes == sizeof(guess_out));

      int guess_i = guess_out;
      if(guess_i != expecteds[i]) error++;
    }  


    timer.stop();
    
    // Report overall error out of all testing instances
    std::cout << "Number of test instances = " << num_test_insts << std::endl;
    std::cout << "Overall Error Rate = " << std::setprecision(3)
              << ( (double)error / num_test_insts ) * 100
              << "%" << std::endl;
 
    // Close input file for the testing set
    myfile.close();
  }
  else
      std::cout << "Unable to open file for the testing set!" << std::endl; 
  
  return 0;
}
