#!/usr/bin/env python
"""mapper.py"""

import sys

k = 9

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        # write the results to STDOUT (standard output);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        # print('{0}\t{1}'.format(word, 1))
        for i in range(len(word)-k+1):
            print('{0}\t{1}'.format(word[i:i+k], 1))
