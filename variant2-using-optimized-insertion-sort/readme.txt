//Tested in https://www.onlinegdb.com
//Published on zenodo.org: 10.5281/zenodo.23626176

LEAN ALGORITHM: Running Median Using Optimized Adaptive Insertion Sort

This project provides an optimized C implementation of a running median algorithm 
for a fixed-size window of a data stream using optimized Insertion Sort.

For each new stream value, the oldest value in the window is replaced. 
The algorithm compares the new and replaced values with the current median 
to determine whether recalculation is necessary. When both values are on the 
same side of the median, the median remains unchanged and no sorting is required. 
Otherwise, an adaptive Insertion Sort algorithm is used to update the ordering 
and determine the new median.

The optimization uses an auxiliary index array for sorting, leaving the original 
data buffer unchanged. 
By taking advantage of the existing order of the index array, 
the algorithm can reduce the number of operations required to calculate the median.

The implementation supports integer and with minor modifications float data types 
and is intended for testing, comparison, and educational purposes. 

The included test program generates pseudo-random input values and displays the 
window contents, sorted values, calculated median, and number of steps counted 
during median calculation.

Key features
- Avoids sorting when the replacement does not change the median; O(1) calculation path.
- Uses an auxiliary index array instead of sorting the original data buffer.
- Takes advantage of the existing order of the index array.
- Supports a fixed-size window with an odd number of values.
- Includes a step counter for performance analysis.

Complexity
- Best-case time:    O(1)
- Worst-case time:   O(N)
- Additional memory: O(N) for the index array.
Here, N denotes the number of values in the fixed-size window.
