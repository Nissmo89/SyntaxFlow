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
    def scheduleCourse(self, courses: list[list[int]]) -> int:
        # Sort courses by deadline (lastDay)
        courses.sort(key=lambda x: x[1])
        
        # Max-heap to store durations. 
        # Python's heapq is a min-heap, so we store negative values to simulate a max-heap.
        max_heap = []
        total_time = 0
        
        for duration, deadline in courses:
            total_time += duration
            heapq.heappush(max_heap, -duration)
            
            # If we exceed the deadline, drop the longest course taken so far
            if total_time > deadline:
                # Subtracting a negative value adds the positive duration back to total_time
                longest_duration = -heapq.heappop(max_heap)
                total_time -= longest_duration
                
        return len(max_heap)
