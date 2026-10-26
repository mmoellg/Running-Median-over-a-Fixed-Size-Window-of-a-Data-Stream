# LEAN ALGORITHM: Running Median of a Fixed-Size Window
Copyright (c) 2026, M. Møllgaard — MIT License

## Overview

This project presents four C implementations for calculating the running
median of a fixed-size window of values from a data stream. Each new input
value replaces the oldest value in the window, and the median is updated
accordingly.

The algorithms are designed for resource-constrained embedded systems,
where execution time, memory consumption, and compact code are important.

The four variants demonstrate different approaches to calculating the
running median, from sorting-based to optimized nonsorting methods.


## Algorithm Variants

### 1. Basic Insertion Sort

Uses insertion sort to sort a copy of the window values and determine the
median. The original circular-buffer data is preserved.

- Straightforward implementation and easy to verify.
- Sorts the window values for each median calculation.
- Best-case time complexity: O(N).
- Worst-case time complexity: O(N²).
- Memory complexity: O(N) for the window and sorting copy.


### 2. Optimized Insertion Sort

Uses an auxiliary index array to maintain the ordering of the window
values without moving the original buffer elements.

The current median is used as a reference to determine whether the median
needs to change after each replacement. If the median remains unchanged,
the algorithm can complete the update in O(1) time. Otherwise, the
existing index ordering is adjusted to determine the new median.

- Reuses the existing ordering instead of sorting from scratch.
- Sorts indices rather than moving the original values.
- Keeps the original window buffer unchanged.
- Best-case time complexity:  O(1).
- Worst-case time complexity: O(N).
- Memory complexity:          O(N) for the window and index array.


### 3. Basic Nonsorting Algorithm

Calculates the running median without sorting the window or maintaining
a sorted copy. The current median is used as a reference to determine
whether the median needs to change after each replacement.

If the median remains unchanged, the algorithm returns without scanning
the buffer. Otherwise, it searches the unsorted buffer for the nearest
candidate on the required side of the current median.

- Uses an unsorted circular buffer.
- Avoids sorting, shifting, and rearranging buffer elements.
- Best-case time complexity: O(1).
- Worst-case time complexity: O(N).
- Memory complexity: O(N); only one data buffer is required.


### 4. Optimized Nonsorting Algorithm

Extends the basic nonsorting approach by tracking duplicate values
associated with the median and candidate values found during a scan.

The current median is used as a reference to determine whether the median
needs to change after each replacement. Duplicate counters provide
additional information that can avoid some buffer scans and improve
performance for data streams containing repeated values.

- Maintains an unsorted circular buffer.
- Uses duplicate counters to optimize selected updates.
- Avoids sorting, shifting, and rearranging buffer elements.
- Best-case time complexity: O(1).
- Worst-case time complexity: O(N).
- Memory complexity: O(N), plus a small amount of counter state.


## Complexity Comparison

Here, N is the number of values in the fixed-size window.
1. Basic Insertion Sort     O(N)         O(N²)         O(N)
2. Optimized Insertion Sort O(1)         O(N)          O(N) indices
3. Basic Nonsorting         O(1)         O(N)          None beyond window
4. Optimized NonsortingO(1) O(N)          A few counters


Variant                     Best case    Worst case    Additional storage
--------------------------  -----------  ------------  ------------------
Basic Insertion Sort        O(N)         O(N²)         O(N)
Optimized Insertion Sort    O(1)         O(N)          O(N) indices
Basic Nonsorting            O(1)         O(N)          None beyond window
Optimized Nonsorting        O(1)         O(N)          A few counters


## Testing and Performance

The implementations can be tested using the included test programs and
pseudo-random input sequences. A sorting-based reference calculation can
be used to verify the median produced by each variant.

Optional performance counters can record the number of buffer elements
examined and the number of updates completed without a full search.
Measured performance depends on the window size, input sequence, data
type, compiler, and target microcontroller.


## Intended Applications

These algorithms are intended for running-median filtering in embedded
systems and other applications where memory usage, execution time, and
code size matter.

The variants provide a basis for comparing sorting-based and nonsorting
approaches and for selecting an implementation suited to a particular
application.


## License
Released under the MIT License.
