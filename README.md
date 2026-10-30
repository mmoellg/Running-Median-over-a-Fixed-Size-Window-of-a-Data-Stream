LEAN ALGORITHM: Running Median over a Fixed-Size Window of a Stream - 4 Variants

<pre>
The four variants has been published on Zenodo.org:
#1 ¦ Basic     Insertion Sort : https://doi.org/10.5281/zenodo.23601248
#2 ¦ Optimized Insertion Sort : https://doi.org/10.5281/zenodo.23626176
#3 ¦ Basic     Nonsorting     : https://doi.org/10.5281/zenodo.23670710
#4 ¦ Optimized Nonsorting     : https://doi.org/10.5281/zenodo.23671391
Published with full C source code provided under the terms of the MIT License.


#INTRODUCTION

This project presents four different C implementations for calculating the
running median over a fixed-size window of values in a data stream.

The four variants demonstrate different approaches, ranging from basic
sorting-based methods to optimized nonsorting methods. They provide a
basis for comparing these approaches and selecting an implementation
suited to a particular application.

The algorithms are intended for resource-constrained hardware,
particularly embedded microcontrollers, where low memory consumption,
compact code size, and fast execution are important design requirements.

A running median can be used as a filter to suppress noise spikes in
sampled data. Compared with a running-average filter, a running median
filter is generally more effective at removing relatively large,
short-duration spikes while preserving the underlying signal
characteristics.


##CONVENTIONAL APPROACH FOR CALCULATING A RUNNING MEDIAN

Conventional approaches using a full sorting algorithm include:
- Complete re-sorting: The entire window is sorted after each update,
  although only one value is removed and one new value is introduced.
- Maintaining a sorted window: The sorted order is maintained between
  updates, avoiding complete re-sorting but still requiring searches and
  potentially extensive data movement.
- Heaps or balanced search trees: These provide favorable
  asymptotic complexity but require additional data structures,
  memory, and implementation complexity.


##THE LEAN ALGORITHM USING PARTLY- OR NON-SORTING IMPLEMENTATIONS

For a running median calculated over a fixed-size window, each update
removes only one value and introduces one new value. Under these
conditions, recalculating the median from scratch can involve
unnecessary computational overhead.

The lean algorithms presented in this project exploit the relationship
between the outgoing value, the incoming value, and the current median.
They use the current median as a reference to determine whether the
median needs to change after each replacement.

The main characteristics of this approach are:
- Median reference: Uses the current median to determine whether
  the median remains unchanged after replacing one value.
- O(1) update: When the median remains unchanged, the update can
  be completed in O(1) time without scanning or re-sorting the window.
- Selective search: When the median changes, additional work is
  required to determine the new median.
- Nonsorting methods: Use an O(N) buffer scan only when necessary,
  without sorting or rearranging the window values.
- Optimized insertion sort: Uses an auxiliary index array and
  adjusts the existing ordering to determine the new median.
- Memory efficiency: Provides implementations with predictable
  storage requirements and avoids unnecessary data movement.


##TIME AND MEMORY COMPLEXITY OF THE 4 VARIANTS

+----+--------------------------+---------------------+---------+
¦ Id.¦  Algorithm using         ¦        Time         ¦  Memory ¦
¦    ¦                          +------+--------------+         ¦
¦    ¦                          ¦ Best ¦    Worst     ¦         ¦
+----+--------------------------+------+--------------+---------+
¦ #1 ¦ Basic Insertion Sort     ¦ O(N) ¦ O(N²)        ¦  O(N)   ¦
¦ #2 ¦ Optimized Insersion Sort ¦ O(1) ¦ O(N·Log2(N)) ¦  O(N)   ¦
¦ #3 ¦ Basic nonsorting         ¦ O(1) ¦ O(N)         ¦  O(1)   ¦
¦ #4 ¦ Optimized nonsorting     ¦ O(1) ¦ O(N)         ¦  O(1)   ¦
+----+--------------------------+------+--------------+---------+
Where, N is the number of values in the fixed-size window.
Memory complexity for the additional storage besides the fixed-size window.



#VARIANTS DESCRIPTION

Common for the four variants:
- An unsorted circular buffer is used for the original of the fixed-size window.
- Replacing an old value with a new value in the buffer uses O(1) execution time.


##VARIANT-#1: Basic Insertion Sort

Uses basic Insertion Sort to sort a copy of the window values and determine the median. 

Features:
- Straightforward implementation and easy to verify.
- Copies the fixed-size window to a separate sorting buffer for each update.
- Sorts the copy of the window values for each update.


##VARIANT-#2: Optimized Insertion Sort

The Basic Insertion Sort was optimized as follows:
A. Uses the current median as a reference to determine whether the median 
    remains unchanged when a new stream value replaces the oldest value.
    If both values are on the same side of the median, the median remains
    unchanged, resulting in an O(1) update.
B. Uses an auxiliary index array to maintain the ordering of the window values
    without moving the original buffer elements. 
    This can improve calculation time  while preserving the original data buffer.
C. Maintains a partly sorted index array to optimize the adaptive Insertion Sort.
    This reduces the worst-case time complexity from O(N²) to O(N*Log2(N)).

Features:
- Sorts indices rather than sorting the original fixed-size window buffer.
- Reuses the existing ordering instead of sorting from scratch.
- O(1) updates: uses the current median to determine whether the median 
   remains unchanged after replacing one value in the buffer.
- Only sorts the partly sorted buffer when not a O(1) update.
- After each non-O(1) update the indices are fully sorted.
- After each     O(1) update the indices are only partly sorted.


##VARIANT-#3: Basic Nonsorting Algorithm

Calculates the running median without sorting the window or maintaining
a sorted copy. The current median is used as a reference to determine
whether the median needs to change after each replacement.
If the median remains unchanged, the algorithm returns without scanning
the buffer. Otherwise, it searches the unsorted buffer for the nearest
candidate on the required side of the current median.

Features:
- Avoids sorting, shifting, and rearranging buffer elements.
- O(1) updates: uses the current median to determine whether the median 
  remains unchanged after replacing one value in the buffer.
- O(N) updates: the buffer must be searched for the new median


##VARIANT-#4: Optimized Nonsorting Algorithm

Extends the basic nonsorting approach by tracking duplicate values
associated with the median and candidate values found during a scan.
The current median is used as a reference to determine whether the median
needs to change after each replacement. Duplicate counters provide
additional information that can avoid some buffer scans and improve
performance for data streams containing repeated values.

Features:
- Same features as for the basic nonsorting algorithm.
- O(1) updates: additional tracks duplicates of the current median.
- The efficiency improves for streams with duplicate values.
- Is less or equal efficient as variant-3 for streams with few duplicates.


#VARIANTS COMPARISON

##RUNTIME PERFORMANCE AND FREQUENCY OF O(1)-UPDATES COMPARISION

The four variants has been tested using an included test program and
pseudo-random input sequences. The basic Insertion Sort was used as 
reference to verify the median produced by each variant.

Optional performance counters recorded the number of buffer elements
examined and the number of O(1) updates completed without a full search.

The execution time T%  relativ to the basic Insertion Sort was measured.
Unlike theoretical operation counts, runtime measurements include the effects 
of the generated machine instructions and the hardware execution characteristics.
Lower T% values indicates a shorter execution time.

The frequency Q% of O(1) updates was recorded.
For the nonsorting and the optimized Insertion Sort algorithms, the number 
of median updates completed in O(1) time was also recorded.
The Q% values should be interpreted as an indicator of the frequency of 
constant-time updates, rather than as a direct measure of execution speed. 
Higher Q% values indicates calculations of the median without scanning the buffer.

Target: Bare-metal 16-bit Microprocessor, Texas Instruments MSP430FR2355.
Stream simulated with 1000, 16 bit signed integers. 

                           N :   5  13  33  63 ¦  5  13  33  63 ¦
--------------------------+---+----------------+----------------+
Simulated test values     ¦id.¦       T%       ¦       Q%       ¦
--------------------------+---+----------------+----------------+
Pseudo-random             ¦#2 ¦ 69  43  21  11 ¦ 39  44  49  52 ¦
                          ¦#3 ¦ 37  14   5   2 ¦ 39  44  49  52 ¦
                          ¦#4 ¦ 40  15   5   2 ¦ 39  46  52  57 ¦
--------------------------+---+----------------+----------------+
Pseudo-random             ¦#2 ¦ 71  43  21  12 ¦ 32  44  42  48 ¦
identical pairs           ¦#3 ¦ 39  14   5   2 ¦ 32  44  42  48 ¦
                          ¦#4 ¦ 33  10   3   1 ¦ 65  73  73  76 ¦
--------------------------+---+----------------+----------------+
Sustained monotonic       ¦#2 ¦ 85  63  35  21 ¦  0   0   0   0 ¦
increasing change         ¦#3 ¦ 52  27  13   7 ¦  0   0   0   0 ¦
                          ¦#4 ¦ 58  30  14   8 ¦  0   1   2   3 ¦
--------------------------+---+----------------+----------------+
Sustained monotonic       ¦#2 ¦ 77  45  21  12 ¦  0   0   0   0 ¦
decreasing change         ¦#3 ¦ 47  19   7   4 ¦  0   0   0   0 ¦
                          ¦#4 ¦ 53  22   8   4 ¦  0   1   2   3 ¦
--------------------------+---+----------------+----------------+
Continuously monotonic    ¦#2 ¦ 68  42  22  13 ¦ 38  45  50  48 ¦
increasing and decreasing ¦#3 ¦ 37  14   5   3 ¦ 38  45  50  48 ¦
every (N-1)/2 update      ¦#4 ¦ 39  14   5   3 ¦ 38  47  53  51 ¦
--------------------------+---+----------------+----------------+
Where: 
N  = Number of values in the fixed-size window.
T% = Relative execution time compared to algorithm id. #1.
Q% = Relative number of O(1) updates compared to the number of stream values.

id.¦ Running median calculated using the following algorithm
---+--------------------------------------------------------
#1 ¦ Basic     Insertion Sort 
#2 ¦ Optimized Insertion Sort 
#3 ¦ Basic     Nonsorting 
#4 ¦ Optimized Nonsorting 


##THEORETICAL OPERATION-COUNT COMPARISON

The four Running Median algorithms were compared by counting the logical 
operations required to execute their respective code paths. 
The worst case of the algorithm #1!, serves as reference for the comparison.
These operation counts provide a theoretical comparison of the algorithms. 
They should be distinguished from the execution time, which also depends on 
instruction selection, branching, memory access, and compiler optimization.
Lower %f(N) values indicates less operations are executed. 

id.¦   Nsub   ¦N: 5   13    33    63  ¦N: 5     13    33    63   ¦
---+----------+-----------------------+--------------------------+
   ¦ a  b  c  ¦          f(N)         ¦ %f(N) relativ to id.#1!  ¦

WORST CASE:
---+----------+-----------------------+--------------------------+
#1!¦  6 17 13 ¦  416 2424 14724 52674 ¦  100.0 100.0 100.0 100.0 ¦
#2*¦ 21 13 16 ¦  272  960  3113  6865 ¦   65.0  40.0  21.0  13.0 ¦
#3 ¦ 20 17  0 ¦  105  241   581  1091 ¦   25.0  10.0   4.0   2.0 ¦
#4 ¦ 32 11  0 ¦   87  175   395   725 ¦   21.0   7.0   3.0   1.0 ¦

BEST CASE:
---+----------+-----------------------+--------------------------+
#1 ¦  5 21  0 ¦  110  278   698  1328 ¦   26.0  11.0   5.0   3.0 ¦
#2 ¦ 19  0  0 ¦   19   19    19    19 ¦    4.6   0.8   0.1   0.0 ¦
#3 ¦ 15  0  0 ¦   15   15    15    15 ¦    3.6   0.6   0.1   0.0 ¦
#4 ¦ 27  0  0 ¦   27   27    27    27 ¦    6.5   1.1   0.2   0.1 ¦

Where:
N     : Number of values in the fixed-size window used for the calculation.
f(N)  : Total number of operations as a function of N.
%f(N) : f(N) in % relative to the worst case of algorithm id.#1!.
Nsub  : Number of sub-operations for calculating the total number of operations.

f(N)  = a + b·N + c·N²        for all algorithms with exception of id. #2*.
f(N)  = a + b·N + c·N·log2(N) for algorithm id. #2*; due partly sorted data.
%f(N) = 100 * f(N) / f(N of id.#1!).


#CONCLUSION

This project presented three variants of Lean Algorithms for calculating
the running median over a fixed-size window of values in a data stream.
The algorithms were compared with a conventional approach based on
basic Insertion Sort.

The Lean Algorithms use the current median as a reference to determine
whether it remains unchanged after each replacement or whether a new
median must be found. This avoids unnecessary sorting or searching
when the median does not need to change.

The nonsorting algorithms provide an O(1) best-case execution path and
an O(N) worst-case execution path, with only O(1) additional memory
beyond the fixed-size data window. They do not require sorting the
window or shifting its elements.

The experimental results demonstrate that runtime performance and
the frequency of O(1) updates depend on the characteristics of the
input data. Depending on the data pattern, the Lean Algorithms can
substantially reduce processing time compared with sorting the entire
window after every update. This is particularly evident for data
containing repeated values or other favorable value patterns.

The Lean Algorithms are therefore well suited to applications where
low memory consumption, compact code, and efficient execution are
important design requirements, particularly in resource-constrained
embedded systems and microcontrollers. 









*/
