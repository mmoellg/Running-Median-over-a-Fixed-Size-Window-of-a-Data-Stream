//Tested in https://www.onlinegdb.com
//Published on zenodo.org: https://doi.org/10.5281/zenodo.23601248

/***********************************************************************************
LEAN ALGORITHM: Running Median Using Basic Insertion Sort
This project provides a simple C implementation of a running median algorithm 
for a fixed-size window of the most recent data from a stream.

For each new input value from the stream, the oldest value in the window is replaced, 
the window values are copied to a separate buffer, and an Insertion Sort algorithm 
is used to determine the median. 
The original window buffer remains unchanged during sorting.

The implementation supports integer and with minor modifications float data types 
and is intended for testing, comparison, and educational purposes. 
It can serve as a reference for evaluating the execution time and memory requirements 
of alternative running-median algorithms, including algorithms designed for 
resource-constrained embedded systems.

The included test program generates pseudo-random input values and displays the 
window contents, sorted values, calculated median, and number of steps counted 
during median calculation.

Complexity
Best-case time:  O(N)
Worst-case time: O(N²)
Memory:          O(N)
Here, N denotes the number of values in the fixed-size window. 
The window size must be odd so that the median is a single middle value.

License
Copyright © 2026 M. Møllgaard.
This implementation is released under the MIT License: 
Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software 
is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************************/
