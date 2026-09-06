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

from typing import List

class Solution:
    def minKBitFlips(self, nums: List[int], k: int) -> int:
        n = len(nums)
        flipped = 0
        res = 0

        for i in range(n):
            # Expire flip that started k steps ago
            if i >= k and nums[i - k] >= 2:
                flipped ^= 1

            # If effective state is 0, we must flip at index i
            if flipped == nums[i]:
                if i + k > n:
                    return -1
                nums[i] ^= 2  # Mark flip origin
                flipped ^= 1
                res += 1

        return res
