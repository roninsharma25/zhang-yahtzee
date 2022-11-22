#!/bin/bash

export LC_ALL=en_US.UTF-8

unset LM_LICENSE_FILE
export XILINXD_LICENSE_FILE=2100@flex.ece.cornell.edu

# Xilinx Vivado System Edition 2016.2
source /opt/xilinx/xilinx_2016.2/Vivado/2016.2/settings64.sh

git config --global alias.got '!git pull && echo "billy mode" > billy.txt && git add billy.txt && git commit -m "i love billy" -m "no, i am not being silly" && git push && clear'