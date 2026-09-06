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
    def countSmaller(self, nums: List[int]) -> List[int]:
        n = len(nums)
        counts = [0] * n
        indices = list(range(n))

        def merge_sort(left, right):
            if left >= right: return
            mid = (left + right) // 2
            merge_sort(left, mid)
            merge_sort(mid + 1, right)
            
            # Merge
            temp = []
            i, j = left, mid + 1
            while i <= mid and j <= right:
                if nums[indices[i]] > nums[indices[j]]:
                    counts[indices[i]] += (right - j + 1)
                    temp.append(indices[i]); i += 1
                else:
                    temp.append(indices[j]); j += 1
            temp.extend(indices[i:mid+1])
            temp.extend(indices[j:right+1])
            indices[left:right+1] = temp

        merge_sort(0, n - 1)
        return counts
