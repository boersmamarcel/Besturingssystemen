Benchmark of malloc vs. our solution:

# cc benchmark.c solution.c
# ./a.out
Benchmarking Malloc:
Time: 400000 microsec

Benchmarking our solution:
Time: 233333 microsec

# ./a.out
Benchmarking Malloc:
Time: 383334 microsec

Benchmarking our solution:
Time: 250000 microsec

# ./a.out
Benchmarking Malloc:
Time: 416666 microsec

Benchmarking our solution:
Time: 266667 microsec


The results clearly show that our solution in almost twice as fast as the regular malloc implementation.
We also noticed the time command seems more accurate than the gettimeofday() function we used. Although gettimeofday() is precise enough to show difference between the two benchmarked implementations, it clearly is not accurate on microseconds (or even close to accurate). Luckily, our solution was awesome enough not to need such accuracy.
