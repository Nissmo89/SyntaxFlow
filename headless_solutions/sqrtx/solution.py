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
    def mySqrt(self, x: int) -> int:
        if x == 0 or x == 1:
            return x
        start, end = 1, x
        while start <= end:
            mid = start + (end - start) // 2
            square = mid * mid
            if square == x:
                return mid
            elif square < x:
                start = mid + 1
            else:
                end = mid - 1
        return end
