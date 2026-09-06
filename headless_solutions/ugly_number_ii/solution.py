import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def nthUglyNumber(self, n: int) -> int:
        ugly = [0] * n
        ugly[0] = 1
        i2 = i3 = i5 = 0
        for i in range(1, n):
            next_val = min(ugly[i2] * 2, ugly[i3] * 3, ugly[i5] * 5)
            ugly[i] = next_val
            if next_val == ugly[i2] * 2: i2 += 1
            if next_val == ugly[i3] * 3: i3 += 1
            if next_val == ugly[i5] * 5: i5 += 1
        return ugly[n - 1]
