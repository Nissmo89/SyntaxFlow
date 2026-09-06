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
from typing import List

class Solution:
    def minRefuelStops(self, target: int, startFuel: int, stations: List[List[int]]) -> int:
        # Python's heapq is a min-heap, so store negative fuel values for a max-heap behavior
        max_heap = []
        curr_reach = startFuel
        stops = 0
        i = 0
        n = len(stations)

        while curr_reach < target:
            # Add all stations we can reach to our max-heap
            while i < n and stations[i][0] <= curr_reach:
                heapq.heappush(max_heap, -stations[i][1])
                i += 1
            
            # If we cannot reach target and no passed stations remain to refuel
            if not max_heap:
                return -1
            
            # Retroactively refuel at the passed station with maximum gas
            curr_reach += -heapq.heappop(max_heap)
            stops += 1

        return stops
