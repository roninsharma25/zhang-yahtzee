//===========================================================================
// digitrec.h
//===========================================================================
// @brief: This header file defines the interface for the core functions.

#ifndef DIGITREC_H
#define DIGITREC_H

#include<hls_stream.h>

#include "typedefs.h"

// Top function for synthesis
void dut(
    hls::stream<bit32_t> &strm_in,
    hls::stream<pixel> &strm_out
);

void bazinga(baz bazingo, baz bazingum, bit4_t threshold_bit);

#endif
