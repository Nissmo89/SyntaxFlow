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
    def kClosest(self, points: List[List[int]], k: int) -> List[List[int]]:
        # Python's heapq is a min-heap. To simulate a max-heap, 
        # we store negative distances.
        heap = []
        for x, y in points:
            dist = -(x*x + y*y)
            if len(heap) == k:
                heapq.heappushpop(heap, (dist, [x, y]))
            else:
                heapq.heappush(heap, (dist, [x, y]))
        
        return [pair[1] for pair in heap]
