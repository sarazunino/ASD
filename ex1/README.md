# Hybrid QuickSort–SelectionSort
## Overview

This project implements a generic hybrid sorting algorithm that combines:  

- QuickSort for large subarrays  
- SelectionSort for small subarrays  

The algorithm switches strategy depending on a threshold parameter `k`:

- If `k = 0` → pure QuickSort
- If `k > total_number_of_records` → pure SelectionSort
- Otherwise → QuickSort for partitions ≥ k and SelectionSort for partitions < k

The implementation supports sorting generic data types via comparison functions and is applied to a large binary dataset  containing 20 million records.  

---

## Execution times and analysis
We tried executing the algorithm multiple times and we noted down the average execution times for each value of k and field (best times are marked in bold):

| k values \ Field | 1          | 2           | 3           | 4          |
| ---------------- | ---------- | ----------- | ----------- | ---------- |
| 0                | 10,090s    | 21.710s     | 12.124s     | 7.930s     |
| 10               | **9.695s** | 13,129s     | **12.087s** | **7.886s** |
| 50               | 10.917s    | 13.015s     | 12.487s     | 7.923s     |
| 100              | 10.473s    | **12.970s** | 13.015s     | 7.947s     |
| 500              | 13.776s    | 17.111s     | 16.629s     | 7.926s     |
| 1000             | 18.084s    | 38.131s     | 21.593s     | 8.011s     |
| 5000             | 57.285s    | 60.827s     | 61.551s     | 7.998s     |
| 10000            | 117.811s   | 111.927s    | 110.462s    | 8.022s     |
| 100000           | >5 minutes | >5 minutes  | >5 minutes  | >5 minutes |

The results are roughly what one would have expected reading the hypothesis: with sufficiently small arrays the overhead of the QuickSort calls makes the execution slower than SelectionSort. According to our results, 10 is the average best k value for most of the fields, given its execution times.
The hybrid approach is better than the algorithms by themselves: it avoids both the highly inefficient SelectionSort times (as clearly seen in the k = 100000 case, in which the array is mostly sorted by SelectionSort and in which the execution time exceeds the 5-minutes failure threshold) and the overhead of the QuickSort calls (as seen in the k = 0 case, in which the whole array is sorte with QuickSort and the execution time exceeds the best possible time).
The fields are relatively comparable to each other, regarding execution time, but with two exception. The second field, which contains floats, has an optimal k value equal to 100, a significant deviation from the trend. The fourth field, which contins strings, has much faster execution times and with little variance between different k values, apart from the k = 100000 case.
We believe that the failure of the tests with k = 100000 is due to the fact that such a high k value forces the array to be mostly sorted with SelectionSort, which degrades the performance thanks to it being O(n^2). We don't believe that the pivot choice in our QuickSort implementation is a factor in this both because this kind of problem would have emerged with lower k values as well and because the strategies we picked for the pivot selection (those being median of three, which we used in the first three fields, and median of five, which we used in the fourth field) are structured in a way that minimizes the likelihood of the worst case scenario happening.