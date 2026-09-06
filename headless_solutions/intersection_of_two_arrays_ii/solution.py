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
    def intersect(self, nums1: List[int], nums2: List[int]) -> List[int]:
        if len(nums1) > len(nums2):
            return self.intersect(nums2, nums1)

        freq = {}
        for num in nums1:
            freq[num] = freq.get(num, 0) + 1

        ans = []
        for num in nums2:
            if freq.get(num, 0) > 0:
                ans.append(num)
                freq[num] -= 1

        return ans
