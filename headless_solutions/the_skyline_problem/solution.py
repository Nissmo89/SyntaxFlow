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
    def getSkyline(self, buildings: List[List[int]]) -> List[List[int]]:
        events = []
        for L, R, H in buildings:
            events.append((L, -H, R))
            events.append((R, 0, 0))
        events.sort()
        
        res = []
        # (height, end_x)
        live_heap = [(0, float('inf'))]
        for x, neg_h, R in events:
            if neg_h != 0:
                heapq.heappush(live_heap, (neg_h, R))
            while live_heap[0][1] <= x:
                heapq.heappop(live_heap)
            
            curr_h = -live_heap[0][0]
            if not res or res[-1][1] != curr_h:
                res.append([x, curr_h])
        return res
