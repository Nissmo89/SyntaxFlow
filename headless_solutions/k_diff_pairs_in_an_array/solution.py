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

from collections import Counter

class Solution:
    def findPairs(self, nums: list[int], k: int) -> int:
        if k < 0: return 0
        counts = Counter(nums)
        pairs = 0
        for x in counts:
            if k == 0:
                if counts[x] > 1:
                    pairs += 1
            else:
                if x + k in counts:
                    pairs += 1
        return pairs
