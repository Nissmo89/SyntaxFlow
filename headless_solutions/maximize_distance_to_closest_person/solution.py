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
    def maxDistToClosest(self, seats: List[int]) -> int:
        last, max_dist, n = -1, 0, len(seats)
        for i, seat in enumerate(seats):
            if seat == 1:
                if last == -1:
                    max_dist = i
                else:
                    max_dist = max(max_dist, (i - last) // 2)
                last = i
        return max(max_dist, n - 1 - last)
