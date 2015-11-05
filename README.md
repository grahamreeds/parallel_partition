# parallel_partition
An implementation of partition algorithm that runs in parallel

This was sparked by two things. One was a colleague asking me about how to get a subset of large selection of objects that are generally, but not guarenteed to be, sorted and the second was watching the [Parallelizing the C++ Standard Template Library](https://www.youtube.com/watch?v=k6djT0a1q3E).

The first sparked a pair of partitions to get the start and end ranges and then sort those like thus (psuedo-c++):

f = partition(begin(v), end(v), lessthan(d1));
l = partition(f, end(v), lessthan(d2));
sort(f,l);

This was indeed quicker, but not by much.

Watching of the video I agreed that parallelizing was a good thing and it was a good first pass at it.  However I disagreed with the passing of an executor object to every function. Converting code to use a parallel function becomes a non-trivial change.  Really what we want is a zero cost version. But that is in the future.

I wanted to see if I could write a parallel version of the partition algorithm.

Between [en.cppreference](http://en.cppreference.com/w/) and [stackoverflow](http://stackoverflow.com/) I pretty much done it.  There are issues with if the number of objects is less than the number of cores and the use of [`rotate`](http://en.cppreference.com/w/cpp/algorithm/rotate) is sub-optimal if the number of moves is larger than about 50% of the block size when using [`reverse`](http://en.cppreference.com/w/cpp/algorithm/reverse) would be quicker.  These could be fixed (trivially) with a teneray operator and a bit of math, but I didn't want to cloud the issue I was trying to solve.
