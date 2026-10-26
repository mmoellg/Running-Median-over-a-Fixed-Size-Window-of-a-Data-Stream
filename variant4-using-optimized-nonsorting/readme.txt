# LEAN ALGORITHM: Running Median of a Fixed-Size Window of a Data Stream
## using Optimized Nonsorting Algorithm

Copyright (c) 2026, M. Møllgaard — MIT License

## Introduction

A running median filter calculates the median of a fixed-size moving window of
values from a data stream. As each new value arrives, it replaces the oldest
value in the window, and the median is updated for the new window.

Running median filters are useful for suppressing short-duration noise spikes
in sampled data while preserving signal transitions. Compared with a
running-average filter, a median filter is less affected by individual extreme
values and can therefore be advantageous in applications involving impulsive
noise.

This project presents an optimized nonsorting algorithm for calculating the
running median of a fixed-size window. The algorithm is designed for
resource-constrained embedded systems, where execution time, memory consumption,
and compact code are important considerations.

Unlike sorting-based implementations, the algorithm does not sort the window
values or maintain them in sorted order. It stores the window in a circular
buffer and uses the current median as a reference for determining whether the
median must change after a new value replaces the oldest one.

The optimized version tracks duplicate values associated with the median and
uses this information to avoid some buffer scans. This can reduce the number
of operations for data streams containing repeated values.

The implementation supports integer and, with minor modifications,
floating-point data types. It is intended for testing, comparison, and
educational purposes.

## Key Features
- Optimized nonsorting algorithm; the buffer remains unsorted.
- Fixed-size sliding window implemented as a circular buffer.
- Uses the current median as a reference for each update.
- Tracks duplicate values on either side of the current median.
- Avoids sorting, shifting, or rearranging buffer elements.
- Best-case time complexity:  O(1).
- Worst-case time complexity: O(N).
- Memory complexity: O(N); only one data buffer is required.
- Duplicate counters can avoid some buffer scans.
- Supports integer and floating-point data types.
- Designed for resource-constrained embedded systems.
- Includes optional performance counters for testing and analysis.
- Open-source C implementation under the MIT License.


## Operating Principle

The algorithm maintains the following elements:
- A circular buffer containing the values in the fixed-size window.
- An index identifying the oldest value to be replaced.
- The current median of the window.
- Counters used to track duplicate values associated with the median
  and candidate values found during a scan.

For a window containing an odd number of values, the median is the middle
value when the window is considered in sorted order. The algorithm does not
need to construct that sorted order. Instead, it uses the current median and
the relative positions of values to determine whether the median can remain
unchanged after an update.

Values are classified as being below, equal to, or above the current median.
The optimized algorithm maintains counters that help determine whether a
replacement can be handled without scanning the buffer.

When a new stream value arrives, it replaces the oldest value in the circular
buffer. The algorithm compares the new value and the replaced value with the
current median. If the replacement cannot change the median, the function
returns immediately. Otherwise, it searches the unsorted buffer for the
nearest candidate on the required side of the current median.


## Median Calculation

The algorithm has three principal cases.

### CASE 1 — Median unchanged: O(1)
If the new value and the replaced value are equal, the window's value
distribution is unchanged. The median therefore remains unchanged.

The median can also remain unchanged when the new value equals the current
median, or when the new and replaced values are on the same side of the
median. The duplicate counters provide additional information that can
identify some other replacements that leave the median unchanged.

In these situations, the algorithm returns without scanning the buffer.
The update therefore requires constant time, O(1), excluding fixed function
call and buffer-update overhead.

### CASE 2 — Search above the median: O(N)
If the new value is above the current median while the replaced value is
below it, the balance of values around the median may change. Certain
replacements involving the median itself can also require a search.

The algorithm scans the unsorted buffer for the smallest value strictly
greater than the current median. During the scan, it tracks occurrences of
the current candidate so that the duplicate counter can be updated.

After the scan, the closest candidate above the current median becomes the
new median.

### CASE 3 — Search below the median: O(N)
The opposite situation occurs when the new value is below the current median
while the replaced value is above it. Certain replacements involving the
median itself can also require a search.

The algorithm scans the buffer for the largest value strictly less than
the current median. During the scan, it tracks occurrences of the current
candidate so that the duplicate counter can be updated.

After the scan, the closest candidate below the current median becomes the
new median.

In both search cases, the buffer remains unsorted, and no buffer elements are
shifted or rearranged.

## CASE2 and CASE3: Duplicate Tracking
The optimized algorithm uses duplicate counters to avoid some buffer scans
when the median remains unchanged after a replacement.

When a new value equals the current median, or when a replacement changes
the balance of values around the median, the counters help determine whether
the median can remain unchanged.

When a scan is required, the algorithm searches for the nearest candidate
above or below the current median. It also tracks repeated occurrences of
that candidate while scanning the buffer.

This optimization is particularly useful for data streams containing
repeated values. The actual performance benefit depends on the input sequence
and the distribution of values in the window.


## Time and Memory Complexity

The fixed-size window is stored in a buffer containing an odd number N of
values.

The algorithm has the following theoretical complexity:

Time complexity: Replace the oldest buffer value  O(1)
Time complexity: Median calculation — best case   O(1)
Time complexity: Median calculation — worst case  O(N)
Memory complexity: (one buffer)                   O(N)

The best case occurs when the median can be retained without scanning the
buffer. In the worst case, the algorithm examines the entire window to
determine the new median.

Only one buffer is required for the window values. No sorted copy, auxiliary
index array, or additional value buffer is needed. The duplicate counters
require only a small amount of additional state.

Although the worst-case time complexity is linear, the actual execution time
depends on the data sequence and the number of buffer elements examined.
Repeated values may allow the algorithm to avoid scans, whereas some
increasing or decreasing sequences may require extensive scanning.


## Implementation and Testing

The C implementation is provided as `RunMedianNoSortOpt()`. The buffer,
current median, circular index, duplicate counters, and related state are
maintained in the `typRunMedianNoSort` structure.

The included test program generates pseudo-random input values and displays
the window contents, sorted values, calculated median, and number of buffer
elements examined during median calculation.

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

This optimized nonsorting running-median algorithm improves on the basic
nonsorting approach by tracking duplicate values and using that information
to avoid some buffer scans. It maintains an unsorted circular buffer and
updates the median without sorting or rearranging the window values.

Its main advantages are low memory requirements, a constant-time best case,
and a linear-time worst case. Duplicate tracking can improve performance
for data streams containing repeated values, making the algorithm relevant
to compact implementations on resource-constrained embedded systems.

##License
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
