# Original research

While most of **cpp-sort**'s algorithms are actually taken from other open-source
projects, and/or naive implementations of well-known sorting algorithms, it also
contains a bit of original research. Most of the things described here are far from
being ground-breaking discoveries, but even slight improvements deserve to at least
be documented so that they can be reused, so... here we go.

### Vergesort

The vergesort is a new sorting algorithm which combines merge operations on almost
sorted data, and falls back to a [pattern-defeating quicksort](https://github.com/orlp/pdqsort)
when the data is not sorted enough. Just like TimSort, it achieves linear time on
some patterns, generally for almost sorted data, and should never be worse than
O(n log n) in the worst case. This last statement has not been proven, but the many
benchmarks show that it is only slightly slower than a pattern-defeating quicksort
for shuffled data, which is its worst case, so...

    Best        Average     Worst       Memory      Stable
    n           n log n     n log n     n           No

While vergesort has been designed to work with random-access iterators, there also
exists a version that works with bidirectional iterators. The algorithm is slightly
different and a bit slower. Basically, it falls back on a regular quicksort instead
of a pattern-defeating quicksort since the latter has to fall back to a heapsort,
which only works with random-access iterators. The complexity for the bidirectional
iterators version is as follows:

    Best        Average     Worst       Memory      Stable
    n           n log n     n²          n           No

This sorting algorithm has been split as a separate project. You can read more
about in the [dedicated repository](https://github.com/Morwenn/vergesort).

### Double insertion sort

The *double insertion sort* is a somewhat novel sorting algorithm to which I gave
what is probably the worst name ever, but another name couldn't describe better
what the algorithm does (honestly, I would be suprised if it hadn't been discovered
before, but I couldn't find any information about it anywhere). The algorithm is
rather simple: it sorts everything but the first and last elements of the array,
switches the first and last elements if they are not ordered, then it inserts the
first element into the sorted sequence from the front and inserts the last element
from the back.

As the regular insertion sort, it comes with two flavours: the version that uses a
liner search to insert the elements and the one that uses a binary search instead.
While the binary search version offers no advatange compared to a regular binary
insertion sort, the linear version has one adavantage compared to the basic linear
insertion sort: even in the worst case, it shouldn't take more than *n* comparisons
to insert both values, where *n* is the size of the collection to sort, making less
comparisons and less moves than a regular insertion sort on average.

    Best        Average     Worst       Memory      Stable      Iterators
    n           n²          n²          1           No          Bidirectional

This algorithm was originally used in **cpp-sort** to generate small array sorts,
but it has since been mostly dropped in favour of better alternatives depending on
the aim of the sort.

### Sorting networks 23 and 24

While trying to reimplement size-optimal sorting networks as described by [*Finding
Better Sorting Networks*](https://etd.ohiolink.edu/!etd.send_file?accession=kent1239814529),
I ended up implementing a sorting network of size 24 that whose size was equivalent
to that of the one described in the paper (123 compare-exchange units). However, it
seems that this sorting network does not use an odd-even merge network but another
merge network. From this network, it was also trivial to generate the corresponding
network of size 23, whose size also corresponds to the best-known one (118). Here
are the two networks and the corresponding sequences of indices:

![Sorting network 23](http://i.imgur.com/gH4JnFP.png)

    [[0, 1],[2, 3],[4, 5],[6, 7],[8, 9],[10, 11],[12, 13],[14, 15],[16, 17],[18, 19],[20, 21]]
    [[1, 3],[5, 7],[9, 11],[0, 2],[4, 6],[8, 10],[13, 15],[17, 19],[12, 14],[16, 18],[20, 22]]
    [[1, 2],[5, 6],[9, 10],[13, 14],[17, 18],[21, 22]]
    [[1, 5],[6, 10],[13, 17],[18, 22]]
    [[5, 9],[2, 6],[17, 21],[14, 18]]
    [[1, 5],[6, 10],[0, 4],[7, 11],[13, 17],[18, 22],[12, 16]]
    [[3, 7],[4, 8],[15, 19],[16, 20]]
    [[0, 4],[7, 11],[12, 16]]
    [[1, 4],[7, 10],[3, 8],[13, 16],[19, 22],[15, 20]]
    [[2, 3],[8, 9],[14, 15],[20, 21]]
    [[2, 4],[7, 9],[3, 5],[6, 8],[14, 16],[19, 21],[15, 17],[18, 20]]
    [[3, 4],[5, 6],[7, 8],[15, 16],[17, 18],[19, 20]]
    [[0, 12],[1, 13],[2, 14],[3, 15],[4, 16],[5, 17],[6, 18],[7, 19],[8, 20],[9, 21],[10, 22]]
    [[2, 12],[3, 13],[10, 20],[11, 21]]
    [[4, 12],[5, 13],[6, 14],[7, 15],[8, 16],[9, 17],[10, 18],[11, 19]]
    [[8, 12],[9, 13],[10, 14],[11, 15]]
    [[6, 8],[10, 12],[14, 16],[7, 9],[11, 13],[15, 17]]
    [[1, 2],[3, 4],[5, 6],[7, 8],[9, 10],[11, 12],[13, 14],[15, 16],[17, 18],[19, 20],[21, 22]]

![Sorting network 24](http://i.imgur.com/qjQrWgj.png)

    [[0, 1],[2, 3],[4, 5],[6, 7],[8, 9],[10, 11],[12, 13],[14, 15],[16, 17],[18, 19],[20, 21],[22, 23]]
    [[1, 3],[5, 7],[9, 11],[0, 2],[4, 6],[8, 10],[13, 15],[17, 19],[21, 23],[12, 14],[16, 18],[20, 22]]
    [[1, 2],[5, 6],[9, 10],[13, 14],[17, 18],[21, 22]]
    [[1, 5],[6, 10],[13, 17],[18, 22]]
    [[5, 9],[2, 6],[17, 21],[14, 18]]
    [[1, 5],[6, 10],[0, 4],[7, 11],[13, 17],[18, 22],[12, 16],[19, 23]]
    [[3, 7],[4, 8],[15, 19],[16, 20]]
    [[0, 4],[7, 11],[12, 16],[19, 23]]
    [[1, 4],[7, 10],[3, 8],[13, 16],[19, 22],[15, 20]]
    [[2, 3],[8, 9],[14, 15],[20, 21]]
    [[2, 4],[7, 9],[3, 5],[6, 8],[14, 16],[19, 21],[15, 17],[18, 20]]
    [[3, 4],[5, 6],[7, 8],[15, 16],[17, 18],[19, 20]]
    [[0, 12],[1, 13],[2, 14],[3, 15],[4, 16],[5, 17],[6, 18],[7, 19],[8, 20],[9, 21],[10, 22],[11, 23]]
    [[2, 12],[3, 13],[10, 20],[11, 21]]
    [[4, 12],[5, 13],[6, 14],[7, 15],[8, 16],[9, 17],[10, 18],[11, 19]]
    [[8, 12],[9, 13],[10, 14],[11, 15]]
    [[6, 8],[10, 12],[14, 16],[7, 9],[11, 13],[15, 17]]
    [[1, 2],[3, 4],[5, 6],[7, 8],[9, 10],[11, 12],[13, 14],[15, 16],[17, 18],[19, 20],[21, 22]]
