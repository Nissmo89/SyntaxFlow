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
    def reversePairs(self, nums: List[int]) -> int:
        def merge_sort(l, r):
            if l >= r: return 0
            mid = (l + r) // 2
            count = merge_sort(l, mid) + merge_sort(mid + 1, r)
            
            j = mid + 1
            for i in range(l, mid + 1):
                while j <= r and nums[i] > 2 * nums[j]:
                    j += 1
                count += (j - (mid + 1))
            
            nums[l:r+1] = sorted(nums[l:r+1])
            return count
            
        return merge_sort(0, len(nums) - 1)
