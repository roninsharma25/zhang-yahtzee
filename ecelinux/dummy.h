//===========================================================================
// dummy.h
//===========================================================================
// @brief: This header file defines the interface for the core functions.

#ifndef DUMMY_H
#define DUMMY_H

#ifndef PIXELS
#define PIXELS 42025
#endif

#ifndef NUMDIE
#define NUMDIE 7
#endif

#include <hls_stream.h>
#include "typedefs.h"

void dut(
    hls::stream<bit32_t> &strm_in,
    hls::stream<bit32_t> &strm_out
);

#endif
