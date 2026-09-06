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
    def findMaximumXOR(self, nums: List[int]) -> int:
        max_xor, mask = 0, 0
        for i in range(30, -1, -1):
            mask |= (1 << i)
            prefixes = {num & mask for num in nums}
            candidate = max_xor | (1 << i)
            for p in prefixes:
                if (p ^ candidate) in prefixes:
                    max_xor = candidate
                    break
        return max_xor
