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


if __name__ == '__main__':
    for pair in oddeven_merge_sort(24):
        print(list(pair))
