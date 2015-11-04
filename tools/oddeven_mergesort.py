import math


###########################################################
# Functions taken from Wikipedia

def oddeven_merge(lo, hi, r):
    step = r * 2
    if step < hi - lo:
        yield from oddeven_merge(lo, hi, step)
        yield from oddeven_merge(lo + r, hi, step)
        yield from [(i, i + r) for i in range(lo + r, hi - r, step)]
    else:
        yield (lo, lo + r)

def oddeven_merge_sort_range(lo, hi):
    """ sort the part of x with indices between lo and hi.

    Note: endpoints (lo and hi) are included.
    """
    if (hi - lo) >= 1:
        # if there is more than one element, split the input
        # down the middle and first sort the first and second
        # half, followed by merging them.
        mid = lo + ((hi - lo) // 2)
        #yield from oddeven_merge_sort_range(lo, mid)
        #yield from oddeven_merge_sort_range(mid + 1, hi)
        yield from oddeven_merge(lo, hi, 1)

def oddeven_merge_sort(length):
    """ "length" is the length of the list to be sorted.
    Returns a list of pairs of indices starting with 0 """
    yield from oddeven_merge_sort_range(0, length - 1)

def compare_and_swap(x, a, b):
    if x[a] > x[b]:
        x[a], x[b] = x[b], x[a]


###########################################################
# Function adapted from Perl's Algorithm::Networksort

def oddeven_merge_sort2(length):
    t = math.ceil(math.log2(length))

    p = 2 ** (t - 1)

    while p > 0:
        q = 2 ** (t - 1)
        r = 0
        d = p

        while d > 0:
            for i in range(length - d):
                if i & p == r:
                    yield (i, i + d)

            d = q - p
            q //= 2
            r = p
        p //= 2


###########################################################
# Index-based recursive approach

def oddeven_merge3(indices):
    # TODO: make it work for networks of arbitrary size
    size = len(indices)

    if size == 2:
        yield(indices[0], indices[1])
    elif size > 1:
        yield from oddeven_merge3([indices[i] for i in range(size) if not i % 2])
        yield from oddeven_merge3([indices[i] for i in range(size) if i % 2])
        for i in range(1, size - 1, 2):
            yield (indices[i], indices[i+1])


###########################################################
# Function adapted from http://flylib.com/books/en/3.55.1.112/1/

#def oddeven_merge2(l: int, m: int, r: int): 
#    N = r - l + 1  # assuming N/2 is m - l + 1 
#    for (int k = N/2; k > 0; k /= 2) 
#        for (int j = k % (N/2); j+k < N; j += k+k) 
#            for i in range(k):
#                yield (l+j+i, l+j+i+k)


if __name__ == '__main__':
    for pair in oddeven_merge3(list(range(14))):
        print(list(pair))

