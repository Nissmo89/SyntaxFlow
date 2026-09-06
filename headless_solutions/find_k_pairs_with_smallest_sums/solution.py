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

import heapq

class Solution:
    def kSmallestPairs(self, nums1: List[int], nums2: List[int], k: int) -> List[List[int]]:
        pq = []
        # Push first k elements of nums1 paired with nums2[0]
        for i in range(min(len(nums1), k)):
            heapq.heappush(pq, (nums1[i] + nums2[0], i, 0))
            
        res = []
        while k > 0 and pq:
            s, i, j = heapq.heappop(pq)
            res.append([nums1[i], nums2[j]])
            if j + 1 < len(nums2):
                heapq.heappush(pq, (nums1[i] + nums2[j + 1], i, j + 1))
            k -= 1
        return res
