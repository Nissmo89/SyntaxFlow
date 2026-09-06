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

import math

class Solution:
    def minEatingSpeed(self, piles: list[int], h: int) -> int:
        left, right = 1, max(piles)
        while left < right:
            mid = (left + right) // 2
            # Calculate total hours needed for speed mid
            hours = sum((p + mid - 1) // mid for p in piles)
            if hours <= h:
                right = mid
            else:
                left = mid + 1
        return left
