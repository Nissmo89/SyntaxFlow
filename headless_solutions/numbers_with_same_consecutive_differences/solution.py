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
    def numsSameConsecDiff(self, n: int, k: int) -> List[int]:
        cur = list(range(1, 10))
        for _ in range(n - 1):
            next_nums = []
            for x in cur:
                y = x % 10
                if y + k <= 9:
                    next_nums.append(x * 10 + y + k)
                if k > 0 and y - k >= 0:
                    next_nums.append(x * 10 + y - k)
            cur = next_nums
        return cur
