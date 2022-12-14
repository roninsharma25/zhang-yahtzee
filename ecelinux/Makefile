#==========================================================================
# Makefile
#==========================================================================
# @brief: A makefile the compiles and synthesizes the digitrec program
#
# @desc: 1. "make" runs csim by default
#        2. "make csim" compiles & executes the fixed-point implementation
#        3. "make clean" cleans up the directory


# Extract Vivado HLS include path
VHLS_PATH := $(dir $(shell which vivado_hls))/..
VHLS_INC ?= ${VHLS_PATH}/include

CFLAGS = -g -I${VHLS_INC} 

all: csim

csim_base: digitrec.cpp digitrec_test.cpp otsu.cpp conn_components.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_base -lrt
	./digitrec_base

csim_alt: digitrec_alt.cpp digitrec_test.cpp otsu.cpp conn_components_alt.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_alt -lrt
	./digitrec_alt

csim_one_base: digitrec.cpp digitrec_base_test_one.cpp otsu.cpp conn_components.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_one -lrt
	./digitrec_one

csim_one_alt: digitrec_alt.cpp digitrec_test_one.cpp conn_components_alt.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_one_alt -lrt
	./digitrec_one_alt

csim_one: digitrec.cpp digitrec_test_one.cpp otsu.cpp conn_components.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_one -lrt
	./digitrec_one

dummy: dummy.cpp dummy_host.cpp
	@echo "Compiling & simulating the dummy on amdpool ..."
	g++ ${CFLAGS} $^ -o dummy_host -lrt
	./dummy_host

csim_two: digitrec_alt.cpp digitrec_test_one.cpp otsu.cpp conn_components.cpp
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o digitrec_two -lrt
	./digitrec_two

bitstream: 
	@echo "Synthesizing digitrec and creating bitstream on amdpool ..."
	vivado_hls -f run_alt.tcl
	./run_bitstream.sh

clean:
	rm -rf digitrec *.dat *.prj *.log
	rm -rf zedboard_project* xillydemo.bit
