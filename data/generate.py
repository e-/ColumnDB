#!/usr/bin/env python
import random

n = 100000

arr1 = [i for i in xrange(n)]
arr2 = [i for i in xrange(n)]

random.shuffle(arr1)
random.shuffle(arr2)

with open('test.csv', 'w') as fp:
    for i in xrange(n):
        print >> fp, '%d,"dummy",%d,"dummy"' % (arr1[i], arr2[i])

