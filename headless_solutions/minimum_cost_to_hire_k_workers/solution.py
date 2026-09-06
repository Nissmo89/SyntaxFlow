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
    def mincostToHireWorkers(self, quality: List[int], wage: List[int], k: int) -> float:
        # Create list of tuples (ratio, quality) and sort by ratio ascending
        workers = sorted([(w / q, q) for q, w in zip(quality, wage)])
        
        max_heap = []  # Python provides min-heap by default; push -q for max-heap
        quality_sum = 0
        min_cost = float('inf')
        
        for ratio, q in workers:
            quality_sum += q
            heapq.heappush(max_heap, -q)
            
            # If heap exceeds size k, evict worker with highest quality
            if len(max_heap) > k:
                quality_sum += heapq.heappop(max_heap)  # Adding negative value subtracts max quality
            
            # Record total wage if group has exactly k workers
            if len(max_heap) == k:
                min_cost = min(min_cost, ratio * quality_sum)
                
        return min_cost
