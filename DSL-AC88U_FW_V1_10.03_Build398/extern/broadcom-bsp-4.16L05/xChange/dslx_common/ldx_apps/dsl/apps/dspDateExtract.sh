#!/bin/bash

# Extracts the date from the standard DSP image date format.
# The first argument should be the DSP header file containing the build date in the header.
#
# For example the DSP archive would contain a line similar to this:
# *     Date:       Fri Apr  3 14:21:04 PDT 2009                                                      
#
# This script would extract the date and represented it as a single string
# without any spaces in the following format:
#
# Fri_Apr_3_2009_14:21:04
# 

grep Date $1| awk '{print $3 "_" $4 "_" $5 "_" $8 "_" $6}'
