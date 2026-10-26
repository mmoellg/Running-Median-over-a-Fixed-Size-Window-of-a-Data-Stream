# LEAN ALGORITHM: Running Median of a Fixed-Size Window of a Data Stream
## Basic Nonsorting Algorithm

Copyright (c) 2026, M. Møllgaard — MIT License

## Introduction

A running median filter calculates the median of a fixed-size moving window of
values from a data stream. As each new value arrives, it replaces the oldest
value in the window, and the median is recalculated for the updated window.

Running median filters are useful for suppressing short-duration noise spikes
in sampled data while preserving signal transitions. Compared with a
running-average filter, a median filter is less affected by individual extreme
values and can therefore be advantageous in applications involving impulsive
noise.

This project presents a basic nonsorting algorithm for calculating the running
median of a fixed-size window. The algorithm is designed with resource-
constrained embedded systems in mind, where execution time, memory consumption,
and compact code are important considerations.

Unlike sorting-based implementations, the algorithm does not sort the window
values or maintain them in sorted order. It stores the window in a circular
buffer and uses the current median as a reference for determining whether the
median must change after a new value replaces the oldest one.

The implementation supports integer and with minor modifications float data types 
and is intended for testing, comparison, and educational purposes.


## Key Features

- Basic nonsorting algorithm; the buffer remains unsorted.
- Fixed-size sliding window implemented as a circular buffer.
- Uses the current median as a reference for each update.
- Avoids sorting, shifting, or rearranging buffer elements.
- Best-case time complexity: O(1).
- Worst-case time complexity: O(N).
- Memory complexity: O(N); only one data buffer is required.
- Early termination when the median remains unchanged.
- Supports integer and floating-point data types.
- Designed for resource-constrained embedded systems.
- Includes optional performance counters for testing and analysis.
- Open-source C implementation under the MIT License.


## Operating Principle

The algorithm maintains three essential elements:
- A circular buffer containing the values in the fixed-size window.
- An index identifying the oldest value to be replaced.
- The current median of the window.

For a window containing an odd number of values, the median is the middle
value when the window is considered in sorted order. The algorithm does not
need to construct that sorted order. Instead, it uses the number of values on
either side of the current median to determine whether the median remains
valid after an update.

Values are classified as being below, equal to, or above the current median.
Values equal to the median are not tracked separately; they are included in
the relevant comparison counts.

When a new stream value arrives, the algorithm first replaces the oldest
value in the circular buffer. It then compares the new value and the replaced
value with the current median. These comparisons establish whether the balance
of values around the median may have changed.


## Median Calculation

The algorithm has three principal cases.

### CASE 1 — Median unchanged: O(1)
If the new value and the replaced value are equal, the window's value
distribution is unchanged. The median therefore remains unchanged.

The median also remains unchanged when the new value equals the current
median. Furthermore, if the new and replaced values are both strictly above
the median, or both strictly below it, the replacement does not change the
number of values on either side of the median.

In these situations, the algorithm returns without scanning the buffer. The
update therefore requires constant time, O(1), excluding any fixed overhead
associated with the function call and buffer update.

### CASE 2 — Search above the median: O(N)
If the new value is above the current median while the replaced value is at
or below it, the balance of values around the median may change.

The algorithm scans the unsorted buffer and counts values at or below the
current median. At the same time, it searches for the smallest value strictly
greater than the current median.

If the required balance is restored before the scan finishes, the current
median remains valid and the algorithm returns early. Otherwise, the closest
candidate above the current median becomes the new median.

### CASE 3 — Search below the median: O(N)
The opposite situation occurs when the new value is below the current median
while the replaced value is at or above it.

The algorithm scans the buffer, counting values at or above the current
median while searching for the largest value strictly less than the current
median.

If the required balance is restored during the scan, the current median
remains valid and the algorithm returns early. Otherwise, the closest
candidate below the current median becomes the new median.

In both search cases, the buffer remains unsorted, and no buffer elements are
shifted or rearranged.


## Exception for CASE2 and CASE3: Early Termination and Duplicate Values

An important feature of the algorithm is its ability to terminate a scan as
soon as sufficient information has been obtained to establish that the current
median remains valid.

Duplicate values, including values equal to the median, affect the balance
counts. They must therefore be handled consistently when determining whether
a scan can terminate early.

The algorithm also allows early termination in some data sequences containing
repeated values. The actual benefit depends on the distribution of the
incoming values and the values already present in the window.


## Time and Memory Complexity

The fixed-size window is stored in a buffer containing an 
odd number N of values.

The algorithm has the following theoretical complexity:

Time complexity: Replace the oldest buffer value  O(1)
Time complexity: Median calculation — best case   O(1)
Time complexity: Median calculation — worst case  O(N)
Memory complexity: (one buffer)                   O(N)           

The best case occurs when the median can be retained without scanning the
buffer. In the worst case, the algorithm examines the entire window to
determine the new median.

Only one buffer is required for the window values. No sorted copy, auxiliary
index array, or additional value buffer is needed.

Although the worst-case time complexity is linear, the actual execution time
depends on the data sequence and the number of buffer elements examined.
Long runs of repeated values may allow early termination, whereas some
sustained increasing or decreasing sequences may require extensive scanning.


## Implementation and Testing

The C implementation is provided as `RunMedianNoSort()`. The buffer, current
median, circular index, and related state are maintained in the
`typRunMedianNoSort` structure.

The included test program generates pseudo-random input values and displays the 
window contents, sorted values, calculated median, and number of steps counted 
during median calculation.

Optional test counters record the number of buffer elements examined during
median calculation and the number of updates that complete without a full
search. These counters can be used to investigate the relationship between
input data characteristics and algorithm performance.

The implementation is intended for embedded applications in which minimizing
additional memory and avoiding sorting operations are important. Its
suitability for a particular microcontroller depends on the window size,
numeric data type, compiler, and required execution time.

The source code is released under the MIT License.


## Conclusion

This basic nonsorting running-median algorithm offers an alternative to
sorting-based methods. By using the current median as a reference and
examining the buffer only when the replacement may change the median, it
avoids maintaining a sorted window and requires only one buffer.

Its main advantages are low memory requirements, a constant-time best case,
and a linear-time worst case. The approach is particularly relevant when
compact implementations and limited memory are priorities in embedded
systems.


## License
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

