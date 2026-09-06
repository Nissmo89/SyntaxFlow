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
    def minSwap(self, nums1: List[int], nums2: List[int]) -> int:
        n = len(nums1)
        keep = 0  # Min swaps if nums1[0] and nums2[0] are NOT swapped
        swap = 1  # Min swaps if nums1[0] and nums2[0] ARE swapped

        for i in range(1, n):
            new_keep = float('inf')
            new_swap = float('inf')

            # Case 1: Natural order is valid
            if nums1[i - 1] < nums1[i] and nums2[i - 1] < nums2[i]:
                new_keep = min(new_keep, keep)
                new_swap = min(new_swap, swap + 1)

            # Case 2: Crossed order is valid
            if nums1[i - 1] < nums2[i] and nums2[i - 1] < nums1[i]:
                new_keep = min(new_keep, swap)
                new_swap = min(new_swap, keep + 1)

            keep, swap = new_keep, new_swap

        return min(keep, swap)
