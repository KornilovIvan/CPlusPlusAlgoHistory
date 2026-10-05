from random import randint


def merge_sort(a, left, right):
    if (right - left) > 1:
        mid = (right + left) // 2
        merge_sort(a, left, mid)
        merge_sort(a, mid, right)
        merge(a, left, right, mid)
    return a


def merge(a, left, right, mid):
    b = [0] * (right - left)
    i = left
    j = mid
    k = 0
    while i < mid or j < right:
        if i < mid and (j == right or a[i] <= a[j]):
            b[k] = a[i]
            i += 1
            k += 1
        else:
            b[k] = a[j]
            j += 1
            k += 1
    for i in range(left, right):
        a[i] = b[i - left]


def qsort(a, left, right):
    if (right - left) > 1:
        x = a[randint(left, right-1)]
        i = left
        j = right - 1
        while i <= j:
            while a[i] < x:
                i += 1
            while a[j] > x:
                j -= 1
            if i <= j:
                a[i], a[j] = a[j], a[i]
                i += 1
                j -= 1
        qsort(a, left, j+1)
        qsort(a, i, right)
    return a


def kth(a, left, right, k):
    if (right - left) == 1:
        return a[left]
    x = a[randint(left, right-1)]
    i = left
    j = right - 1
    while i <= j:
        while a[i] < x:
            i += 1
        while a[j] > x:
            j -= 1
        if i <= j:
            a[i], a[j] = a[j], a[i]
            i += 1
            j -= 1
    if k < j:
        return kth(a, left, j, k)
    elif k < i:
        return x
    else:
        return kth(a, i, right, k)


def binsearch(a, x):
    left = -1
    right = len(a)
    while (right - left) > 1:
        mid = left + (right - left) // 2
        if a[mid] >= x:
            right = mid
        else:
            left = mid
    return right


def sift_up(h, pos):
    while pos > 0 and h[pos] > h[(pos-1) // 2]:
        h[pos], h[(pos - 1) // 2] = h[(pos - 1) // 2], h[pos]
        pos = (pos - 1) // 2


def heapify(h, pos, n):
    while 2 * pos + 1 < n:
        c = 2 * pos + 1
        if c + 1 < n and h[c+1] > h[c]:
            c += 1
        if h[c] > h[pos]:
            h[c], h[pos] = h[pos], h[c]
            pos = c
        else:
            break


def heap_sort(a, n):
    while n > 1:
        a[0], a[n-1] = a[n-1], a[0]
        n -= 1
        heapify(a, 0, n)


a = list(map(int, input().split()))
print(binsearch(a, 5))