//==========================================================================
//digitrec.cpp
//==========================================================================
// @brief: A k-nearest-neighbor implementation for digit recognition (k=1)

#include "digitrec.h"
#include "otsu.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------

int first = 1;
int histogram[256];
int count = 0;

void dut(
    hls::stream<bit32_t> &strm_in,
    hls::stream<pixel> &strm_out
)
{
  // -----------------------------
  // YOUR CODE GOES HERE
  // -----------------------------

  digit test_digit;
  digit result;
  
  if(first){
    for (int i = 0; i < 256; i++){
      histogram[i] = 0;
    }
    first = 0;
  }

  // ------------------------------------------------------
  // Input processing
  // ------------------------------------------------------
  // Read the two input 32-bit words (low word first)
  bit32_t input_lo = strm_in.read();
  // Set digit properly
  test_digit(31, 0) = input_lo;



  // Update the histogram
  update_histogram(test_digit, histogram);
  

  // printf("histogram: [");
  //   for(int i = 0; i < 256; i++){
  //     printf(" %d ", histogram[i]);
  //   }
  // printf("]");

  // Invoke Otsu's algorithm
  // hls::stream<bit32_t> &otsu;
  count++;



  // printf("awooga %d, %d\n", input_lo.to_int(), result.to_int() );
  
  // Write out the interpreted digit
  if(count >= 2800){
    printf("histogram: [");
      for(int i = 0; i < 256; i++){
        printf("%d,", histogram[i]);
      }
    printf("]");
    pixel threshold = otsu(histogram);
    strm_out.write(threshold);
  }

}

//----------------------------------------------------------
// Digitrec
//----------------------------------------------------------
// @param[in] : input - the testing instance
// @return : the recognized digit (0~9)

bit4_t digitrec( digit input ) 
{
  #include "training_data.h"

  // This array stores K minimum distances per training set
  bit6_t knn_set[10][K_CONST];

  // Initialize the knn set
  for ( int i = 0; i < 10; ++i )
    for ( int k = 0; k < K_CONST; ++k )
      // Note that the max distance is 49
      knn_set[i][k] = 50; 

  L1800: for ( int i = 0; i < TRAINING_SIZE; ++i ) {
    L10: for ( int j = 0; j < 10; j++ ) {
      // Read a new instance from the training set
      digit training_instance = training_data[j][i];
      // Update the KNN set
      update_knn( input, training_instance, knn_set[j] );
    }
  } 

  // Compute the final output
  return knn_vote( knn_set ); 
}


//-----------------------------------------------------------------------
// update_knn function
//-----------------------------------------------------------------------
// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.

// @param[in] : test_inst - the testing instance
// @param[in] : train_inst - the training instance
// @param[in/out] : min_distances[K_CONST] - the array that stores the current
//                  K_CONST minimum distance values per training set

void update_knn( digit test_inst, digit train_inst, bit6_t min_distances[K_CONST] )
{
  // Compute the difference using XOR
  digit diff = test_inst ^ train_inst;
  bit6_t dist = 0;
  // Count the number of set bits
  for ( int i = 0; i < 49; ++i ) { 
    dist += diff[i];
  }

  // -----------------------------
  // YOUR CODE GOES HERE
  // -----------------------------

  bit6_t largest_distance_index = 0;
  UPDATE_LOOP_2:
  for (int k = 0; k < K_CONST; k++){
    if (min_distances[largest_distance_index] < min_distances[k]){ 
      largest_distance_index = k;
    }
  }
  if (min_distances[largest_distance_index] > dist)
    min_distances[largest_distance_index] = dist;
  return;
}

//-----------------------------------------------------------------------
// knn_vote function
//-----------------------------------------------------------------------
// Given 10xK minimum distance values, this function 
// finds the actual K nearest neighbors and determines the
// final output based on the most common digit represented by 
// these nearest neighbors (i.e., a vote among KNNs). 
//
// @param[in] : knn_set - 10xK_CONST min distance values
// @return : the recognized digit
// 


bit4_t knn_vote( bit6_t knn_set[10][K_CONST] )
{
  // The index matches the classification of a digit with the distance
  // to the test point. The classifications are kept in k_closest_class
  // and the distances are kept in k_closest_distances.
  CLOSEST_CLASS:
  bit4_t k_closest_class[K_CONST];
  CLOSEST_DISTANCE:
  bit6_t k_closest_distances[K_CONST];
  for (int i = 0; i < K_CONST; i++){
    k_closest_class[i] = 0;
    k_closest_distances[i] = 50;
  }
  bit6_t largest_distance_index;
  // Initialize closest distance and class arrays.
  VOTE_LOOP_1:
  for (int i = 0; i < 10; i++){
    VOTE_LOOP_2:
    for (int j = 0; j < K_CONST; j++){
      // Figure out the largest distance inside k_closest_distances.
      largest_distance_index = 0;
      VOTE_LOOP_3:
      for (int k = 0; k < K_CONST; k++){
        if (k_closest_distances[largest_distance_index] < k_closest_distances[k]){ 
          largest_distance_index = k;
        }
      }
      // If the distance of knn_set is less than the largest distance
      // in k_closest_distances, put the distance in k_closest_distances
      // and the classification of the digit in k_closest_class.
      // Use the same index for both arrays.
      if (k_closest_distances[largest_distance_index] > knn_set[i][j]){
        k_closest_distances[largest_distance_index] = knn_set[i][j];
        k_closest_class[largest_distance_index] = i;
      }
    }
  }

  // Voting algorithm
  // The predicted digit.
  bit4_t candidate;
  // How many elements in the k_closest_class array align with 
  // the candidate prediction.
  bit6_t candidate_votes = 0;
  // The closest distance out of any of the candidate votes.
  bit6_t candidate_closest = 50;
  // How many elements in the k_closest_class array align with
  // the current value of i. 
  bit4_t num_votes;
  // The closest distance out of any of the votes for i.
  bit6_t closest;
  VOTE_LOOP_4:
  for (int i = 0; i < 10; i++){
    // Reset votes and closest distance
    num_votes = 0;
    closest = 50;
    VOTE_LOOP_5:
    for (int j = 0; j < K_CONST; j++){
      // If an element matches the classification given by i,
      // add a vote to num_Votes.
      if (i == k_closest_class[j]){
        num_votes++;
        // If this element is the closest out of all the votes
        // for this number, set closest.
        if (closest > k_closest_distances[j])
          closest = k_closest_distances[j];
      } 
    }
    // If the number of votes exceeds the number of candidate votes,
    // or if the number of votes are equal but the closest distance
    // of i is less than the closest distance of the candidate,
    // set the candidate and all associated variables to whatever 
    // corresponds with i.
    
    //std::cout << "\nPrevious candidate: " << candidate << ", Next candidate: " << i;
    //std::cout << "\nPrevious votes: " << candidate_votes << ", Next votes: " << num_votes;
    //std::cout << "\nPrevious closest: " << candidate_closest << ", Next closest: " << closest << "\n";

    if (num_votes > candidate_votes ||
      (num_votes == candidate_votes && closest < candidate_closest)){
      //std::cout << "\nCandidate selected\n";
      candidate = i;
      candidate_votes = num_votes;
      candidate_closest = closest;
    }
  }
  // Return the selected candidate.
  return candidate;
}