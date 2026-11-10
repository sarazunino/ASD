# Exercise 3 — Ternary Heap

## Implementation

The priority queue is backed by a **max ternary heap** stored in a dynamically resized array. Each node has up to three children, with indices derived as:
 
- **Parent** of node `i`: `(i - 1) / 3`
- **First child** of node `i`: `3 * i + 1`

`push` and `pop` call `sift_up` and `sift_down` respectively. `sift_down` selects the highest-priority child among up to three candidates at each level. All core operations run in **O(log n)**.
 
To support `contains` and `remove` in sub-linear time, a **hash table** (`pos_map`) maps each element to its current index in the heap array. Every `swap` updates both entries. 

The hash function passed to `priority_queue_create` is used directly by the internal hash table. In the scheduler, each `Task`'s pointer address is used as its hash key (`(unsigned long)(uintptr_t)elem`), which is unique since all tasks live in a single contiguous array.

When two tasks share the same priority, the scheduler breaks the tie by `start` time, the task that arrived earlier is preferred, implementing a FIFO policy among equal-priority tasks. If both `priority` and `start` are equal, pointer address is used as a final tiebreaker to ensure `compare` never returns 0 for distinct tasks, which is required for correct hash table behaviour.

## Results
 
The simulation was run on `tasks.csv` containing **1,000,000 tasks**.
 
| Phase      | Time    |
|------------|---------|
| Scheduling | 0.311 s |
 
The measured time is consistent with the expected **O(n log n)** complexity, as the simulation performs n insertions and n extractions on the priority queue, each taking O(log n) time.