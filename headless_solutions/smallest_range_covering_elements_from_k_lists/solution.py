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
    def smallestRange(self, nums: List[List[int]]) -> List[int]:
        pq = [(row[0], i, 0) for i, row in enumerate(nums)]
        heapq.heapify(pq)
        
        cur_max = max(row[0] for row in nums)
        ans = [-float('inf'), float('inf')]
        
        while pq:
            min_val, r, c = heapq.heappop(pq)
            if cur_max - min_val < ans[1] - ans[0]:
                ans = [min_val, cur_max]
            
            if c + 1 < len(nums[r]):
                next_val = nums[r][c + 1]
                heapq.heappush(pq, (next_val, r, c + 1))
                cur_max = max(cur_max, next_val)
            else:
                break
        return ans
