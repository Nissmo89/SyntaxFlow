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
    def preimageSizeFZF(self, k: int) -> int:
        def count_zeroes(x: int) -> int:
            count = 0
            while x >= 5:
                x //= 5
                count += x
            return count
        
        def get_first(target: int) -> int:
            low, high = 0, 5 * target + 5
            while low < high:
                mid = (low + high) // 2
                if count_zeroes(mid) < target:
                    low = mid + 1
                else:
                    high = mid
            return low
            
        left = get_first(k)
        right = get_first(k + 1)
        return int(right - left)
